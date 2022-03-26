#pragma once

#include "matplotlibcpp.h"

#include <Python.h>
#include <datetime.h>

#include <string>
#include <cstdlib>
#include <map>
#include <ranges>
#include <chrono>

// Convenience functions for converting C/C++ time objects to datetime.datetime
// objects. These are outside the matplotlibcpp namespace because they do not
// exist in matplotlib.pyplot.
template<class TimePoint>
PyObject* toPyDateTime(const TimePoint& t, int dummy=0)
{
    using namespace std::chrono;
    auto tsec=time_point_cast<seconds>(t);
    auto us=duration_cast<microseconds>(t-tsec);

    time_t tt=system_clock::to_time_t(t);
    PyObject* obj=toPyDateTime(tt, us.count());

    return obj;
}

template <>
PyObject* toPyDateTime(const time_t& t, int us)
{
    tm tm {};
    gmtime_r(&t,&tm);         // compatible with matlab, inverse of datenum.

    if (!PyDateTimeAPI) {
	PyDateTime_IMPORT;
    }

    PyObject* obj=PyDateTime_FromDateAndTime(tm.tm_year+1900,
					     tm.tm_mon+1,
					     tm.tm_mday,
					     tm.tm_hour,
					     tm.tm_min,
					     tm.tm_sec,
					     us);
    if (obj) {
	PyDateTime_Check(obj);
	Py_INCREF(obj);
    }

    return obj;
}

template <class Time_t>
PyObject* toPyDateTimeList(const Time_t* t, size_t nt)
{
    PyObject* tlist=PyList_New(nt);
    if (tlist==nullptr)
	return nullptr;

    // Py_INCREF(tlist);

    if (!PyDateTimeAPI) {
	PyDateTime_IMPORT;
    }

    for (size_t i=0; i<nt; i++) {
	PyObject* ti=toPyDateTime(t[i], 0);
	PyList_SET_ITEM(tlist, i, ti);
    }

    return tlist;
}

template <class Time_t>
class DateTimeList
{
public:

    DateTimeList(const Time_t* t, size_t nt) {
	tlist=(PyListObject*) toPyDateTimeList(t, nt);
    }

    ~DateTimeList() { if (tlist) Py_DECREF((PyObject*) tlist); }

    PyListObject* get() const { return tlist; }
    size_t size() const { return tlist ? PyList_Size((PyObject*)tlist) : 0; }

private:
    mutable PyListObject* tlist=nullptr;
};



namespace matplotlibcpp {

// special purpose function to plot against python datetime objects.
template <class Time_t, std::ranges::contiguous_range ContainerY>
bool plot(const DateTimeList<Time_t>& t, const ContainerY& y,
	  const std::string& fmt="")
{
    detail::_interpreter::get();

    // DECREF decrements the ref counts of all objects in the plot_args tuple,
    // In particular, it decreasesthe ref count of the time array x.
    // We want to maintain that unchanged though, so we can reuse it.
    PyListObject* tarray=t.get();
    Py_INCREF(tarray);

    NPY_TYPES ytype=detail::select_npy_type<typename ContainerY::value_type>::type;

    npy_intp tsize=PyList_Size((PyObject*)tarray);
    assert(y.size()%tsize == 0 && "length of y must be a multiple of length of x!");

    npy_intp yrows=tsize, ycols=y.size()/yrows;
    npy_intp ysize[]={yrows, ycols};   // ysize[0] must equal tsize

    PyObject* yarray = PyArray_New(&PyArray_Type,
				   2, ysize, ytype, nullptr, (void*) y.data(),
				   0, NPY_ARRAY_FARRAY, nullptr);  // col major

    PyObject* pystring = PyString_FromString(fmt.c_str());

    PyObject* plot_args = PyTuple_New(3);
    PyTuple_SetItem(plot_args, 0, (PyObject*)tarray);
    PyTuple_SetItem(plot_args, 1, yarray);
    PyTuple_SetItem(plot_args, 2, pystring);

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_plot, plot_args);

    Py_DECREF(plot_args);
    if(res) Py_DECREF(res);

    return true;
}

}
