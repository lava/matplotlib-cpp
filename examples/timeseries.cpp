//
// g++ -std=c++20 -g -Wall -o timeseries $(python-config --includes) timeseries.cpp $(python-config --ldflags --embed)
// g++ -std=c++20 -g -Wall -fsanitize=address -o timeseries $(python-config --includes) timeseries.cpp $(python-config --ldflags --embed) -lasan
//
#include "../matplotlibcpp.h"
#include "../datetime_utils.h"

#include <cassert>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;
namespace plt = matplotlibcpp;

// In python2.7 there was a dedicated function PyString_AsString, but no more.
string tostring(PyObject* obj)
{
    string out;
    PyObject* repr=PyObject_Repr(obj);   // unicode object
    if (repr) {
	PyObject* str=PyUnicode_AsEncodedString(repr, 0, 0);
	if (str) {
	    const char* bytes=PyBytes_AS_STRING(str);
	    out=bytes;
	    Py_DECREF(str);
	}

	Py_DECREF(repr);
    }

    return out;
}

int main()
{
    using namespace std::chrono;
    using clk=std::chrono::high_resolution_clock;

    plt::detail::_interpreter::get();       // initialize everything

    // Test toPyDateTime functions.
    PyObject* now=toPyDateTime(time(0));
    cout << tostring(now) << endl;
    Py_DECREF(now);

    now=toPyDateTime(clk::now());
    cout << tostring(now) << endl;
    Py_DECREF(now);


    // Time series plot

    // We have a time array (e.g. from a csv file):
    size_t n=10;
    vector<time_t> tvec;
    time_t tstart=time(0);
    for (size_t i=0; i<n; i++, tstart+=24*3600)
	tvec.push_back(tstart);

    // Create the python array of times - ONCE!
    DateTimeList<time_t> tarray(tvec.data(), tvec.size());

    // Create 2-column data and plot it against tarray:
    vector<double> data(2*n);
    for (size_t i=0; i<2*n; i++) data[i]=2.0*i+5;

    plt::plot(tarray, data);
    plt::xticks({{"rotation", "20"}});
    plt::title("Linear");
    plt::grid(true);
    plt::show();

    // Modify some data and plot again, reusing the time array:
    for (size_t i=0; i<n; i++) data[i]=i*i-3.0;

    // plt::plot((PyListObject*) tarray, data);
    plt::plot(tarray, data);
    plt::xticks({{"rotation", "20"}});
    plt::title("Quadratic");
    plt::grid(true);
    plt::show();

    // Unfortunately, we have to call the DateTimeList destructor explicitly
    // for now, because the destructor needs an interpreter, which we kill
    // on the next line. In turn, that's due to the singleton implementation
    // of the interpreter using a static object.
    //
    // TODO: implement the singleton using a pointer. The pointer
    // (to _interpreter::ctx) is still static, but ctx can be deleted at the
    // right time (before Py_Finalize); it is not at the mercy of the OS.
    tarray.~DateTimeList<time_t>();
    plt::detail::_interpreter::kill();

    return 0;
}
