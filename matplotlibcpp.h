#pragma once

#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <stdint.h> // <cstdint> requires c++11 support

#if __cplusplus > 199711L || _MSC_VER > 1800
#  include <functional>
#endif

#include <Python.h>

#ifndef WITHOUT_NUMPY
#  define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#  include <numpy/arrayobject.h>
#endif // WITHOUT_NUMPY

#if PY_MAJOR_VERSION >= 3
#  define PyString_FromString PyUnicode_FromString
#endif


namespace matplotlibcpp {
namespace detail {

static std::string s_backend;

struct _interpreter {
    PyObject *s_python_function_show;
    PyObject *s_python_function_close;
    PyObject *s_python_function_draw;
    PyObject *s_python_function_pause;
    PyObject *s_python_function_save;
    PyObject *s_python_function_figure;
    PyObject *s_python_function_plot;
    PyObject *s_python_function_quiver;
    PyObject *s_python_function_semilogx;
    PyObject *s_python_function_semilogy;
    PyObject *s_python_function_loglog;
    PyObject *s_python_function_fill_between;
    PyObject *s_python_function_hist;
    PyObject *s_python_function_subplot;
    PyObject *s_python_function_legend;
    PyObject *s_python_function_xlim;
    PyObject *s_python_function_ion;
    PyObject *s_python_function_ginput;
    PyObject *s_python_function_ylim;
    PyObject *s_python_function_title;
    PyObject *s_python_function_axis;
    PyObject *s_python_function_xlabel;
    PyObject *s_python_function_ylabel;
    PyObject *s_python_function_xticks;
    PyObject *s_python_function_yticks;
    PyObject *s_python_function_grid;
    PyObject *s_python_function_clf;
    PyObject *s_python_function_errorbar;
    PyObject *s_python_function_annotate;
    PyObject *s_python_function_tight_layout;
    PyObject *s_python_empty_tuple;
    PyObject *s_python_function_stem;
    PyObject *s_python_function_xkcd;
    PyObject *s_python_function_text;
    PyObject *s_python_function_suptitle;

    /* For now, _interpreter is implemented as a singleton since its currently not possible to have
       multiple independent embedded python interpreters without patching the python source code
       or starting a separate process for each.
        http://bytes.com/topic/python/answers/793370-multiple-independent-python-interpreters-c-c-program
       */

    static _interpreter& get() {
        static _interpreter ctx;
        return ctx;
    }

private:

#ifndef WITHOUT_NUMPY
#  if PY_MAJOR_VERSION >= 3

    void *import_numpy() {
        import_array(); // initialize C-API
        return NULL;
    }

#  else

    void import_numpy() {
        import_array(); // initialize C-API
    }

#  endif
#endif

    _interpreter() {

        // optional but recommended
#if PY_MAJOR_VERSION >= 3
        wchar_t name[] = L"plotting";
#else
        char name[] = "plotting";
#endif
        Py_SetProgramName(name);
        Py_Initialize();

#ifndef WITHOUT_NUMPY
        import_numpy(); // initialize numpy C-API
#endif

        PyObject* matplotlibname = PyString_FromString("matplotlib");
        PyObject* pyplotname = PyString_FromString("matplotlib.pyplot");
        PyObject* pylabname  = PyString_FromString("pylab");
        if (!pyplotname || !pylabname || !matplotlibname) {
            throw std::runtime_error("couldnt create string");
        }

        PyObject* matplotlib = PyImport_Import(matplotlibname);
        Py_DECREF(matplotlibname);
        if (!matplotlib) { throw std::runtime_error("Error loading module matplotlib!"); }

        // matplotlib.use() must be called *before* pylab, matplotlib.pyplot,
        // or matplotlib.backends is imported for the first time
        if (!s_backend.empty()) {
            PyObject_CallMethod(matplotlib, const_cast<char*>("use"), const_cast<char*>("s"), s_backend.c_str());
        }

        PyObject* pymod = PyImport_Import(pyplotname);
        Py_DECREF(pyplotname);
        if (!pymod) { throw std::runtime_error("Error loading module matplotlib.pyplot!"); }


        PyObject* pylabmod = PyImport_Import(pylabname);
        Py_DECREF(pylabname);
        if (!pylabmod) { throw std::runtime_error("Error loading module pylab!"); }

        s_python_function_show = PyObject_GetAttrString(pymod, "show");
        s_python_function_close = PyObject_GetAttrString(pymod, "close");
        s_python_function_draw = PyObject_GetAttrString(pymod, "draw");
        s_python_function_pause = PyObject_GetAttrString(pymod, "pause");
        s_python_function_figure = PyObject_GetAttrString(pymod, "figure");
        s_python_function_plot = PyObject_GetAttrString(pymod, "plot");
        s_python_function_quiver = PyObject_GetAttrString(pymod, "quiver");
        s_python_function_semilogx = PyObject_GetAttrString(pymod, "semilogx");
        s_python_function_semilogy = PyObject_GetAttrString(pymod, "semilogy");
        s_python_function_loglog = PyObject_GetAttrString(pymod, "loglog");
        s_python_function_fill_between = PyObject_GetAttrString(pymod, "fill_between");
        s_python_function_hist = PyObject_GetAttrString(pymod,"hist");
        s_python_function_subplot = PyObject_GetAttrString(pymod, "subplot");
        s_python_function_legend = PyObject_GetAttrString(pymod, "legend");
        s_python_function_ylim = PyObject_GetAttrString(pymod, "ylim");
        s_python_function_title = PyObject_GetAttrString(pymod, "title");
        s_python_function_axis = PyObject_GetAttrString(pymod, "axis");
        s_python_function_xlabel = PyObject_GetAttrString(pymod, "xlabel");
        s_python_function_ylabel = PyObject_GetAttrString(pymod, "ylabel");
        s_python_function_xticks = PyObject_GetAttrString(pymod, "xticks");
        s_python_function_yticks = PyObject_GetAttrString(pymod, "yticks");
        s_python_function_grid = PyObject_GetAttrString(pymod, "grid");
        s_python_function_xlim = PyObject_GetAttrString(pymod, "xlim");
        s_python_function_ion = PyObject_GetAttrString(pymod, "ion");
        s_python_function_ginput = PyObject_GetAttrString(pymod, "ginput");
        s_python_function_save = PyObject_GetAttrString(pylabmod, "savefig");
        s_python_function_annotate = PyObject_GetAttrString(pymod,"annotate");
        s_python_function_clf = PyObject_GetAttrString(pymod, "clf");
        s_python_function_errorbar = PyObject_GetAttrString(pymod, "errorbar");
        s_python_function_tight_layout = PyObject_GetAttrString(pymod, "tight_layout");
        s_python_function_stem = PyObject_GetAttrString(pymod, "stem");
        s_python_function_xkcd = PyObject_GetAttrString(pymod, "xkcd");
        s_python_function_text = PyObject_GetAttrString(pymod, "text");
        s_python_function_suptitle = PyObject_GetAttrString(pymod, "suptitle");

        if(    !s_python_function_show
            || !s_python_function_close
            || !s_python_function_draw
            || !s_python_function_pause
            || !s_python_function_figure
            || !s_python_function_plot
            || !s_python_function_quiver
            || !s_python_function_semilogx
            || !s_python_function_semilogy
            || !s_python_function_loglog
            || !s_python_function_fill_between
            || !s_python_function_subplot
            || !s_python_function_legend
            || !s_python_function_ylim
            || !s_python_function_title
            || !s_python_function_axis
            || !s_python_function_xlabel
            || !s_python_function_ylabel
            || !s_python_function_grid
            || !s_python_function_xlim
            || !s_python_function_ion
            || !s_python_function_ginput
            || !s_python_function_save
            || !s_python_function_clf
            || !s_python_function_annotate
            || !s_python_function_errorbar
            || !s_python_function_errorbar
            || !s_python_function_tight_layout
            || !s_python_function_stem
            || !s_python_function_xkcd
            || !s_python_function_text
            || !s_python_function_suptitle
        ) { throw std::runtime_error("Couldn't find required function!"); }

        if (   !PyFunction_Check(s_python_function_show)
            || !PyFunction_Check(s_python_function_close)
            || !PyFunction_Check(s_python_function_draw)
            || !PyFunction_Check(s_python_function_pause)
            || !PyFunction_Check(s_python_function_figure)
            || !PyFunction_Check(s_python_function_plot)
            || !PyFunction_Check(s_python_function_quiver)
            || !PyFunction_Check(s_python_function_semilogx)
            || !PyFunction_Check(s_python_function_semilogy)
            || !PyFunction_Check(s_python_function_loglog)
            || !PyFunction_Check(s_python_function_fill_between)
            || !PyFunction_Check(s_python_function_subplot)
            || !PyFunction_Check(s_python_function_legend)
            || !PyFunction_Check(s_python_function_annotate)
            || !PyFunction_Check(s_python_function_ylim)
            || !PyFunction_Check(s_python_function_title)
            || !PyFunction_Check(s_python_function_axis)
            || !PyFunction_Check(s_python_function_xlabel)
            || !PyFunction_Check(s_python_function_ylabel)
            || !PyFunction_Check(s_python_function_grid)
            || !PyFunction_Check(s_python_function_xlim)
            || !PyFunction_Check(s_python_function_ion)
            || !PyFunction_Check(s_python_function_ginput)
            || !PyFunction_Check(s_python_function_save)
            || !PyFunction_Check(s_python_function_clf)
            || !PyFunction_Check(s_python_function_tight_layout)
            || !PyFunction_Check(s_python_function_errorbar)
            || !PyFunction_Check(s_python_function_stem)
            || !PyFunction_Check(s_python_function_xkcd)
            || !PyFunction_Check(s_python_function_text)
            || !PyFunction_Check(s_python_function_suptitle)
        ) { throw std::runtime_error("Python object is unexpectedly not a PyFunction."); }

        s_python_empty_tuple = PyTuple_New(0);
    }

    ~_interpreter() {
        Py_Finalize();
    }
};

} // end namespace detail

// must be called before the first regular call to matplotlib to have any effect
inline void backend(const std::string& name)
{
    detail::s_backend = name;
}

inline bool annotate(std::string annotation, double x, double y)
{
    PyObject * xy = PyTuple_New(2);
    PyObject * str = PyString_FromString(annotation.c_str());

    PyTuple_SetItem(xy,0,PyFloat_FromDouble(x));
    PyTuple_SetItem(xy,1,PyFloat_FromDouble(y));

    PyObject* kwargs = PyDict_New();
    PyDict_SetItemString(kwargs, "xy", xy);

    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, str);

    PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_annotate, args, kwargs);

    Py_DECREF(args);
    Py_DECREF(kwargs);

    if(res) Py_DECREF(res);

    return res;
}

#ifndef WITHOUT_NUMPY
// Type selector for numpy array conversion
template <typename T> struct select_npy_type { const static NPY_TYPES type = NPY_NOTYPE; }; //Default
template <> struct select_npy_type<double> { const static NPY_TYPES type = NPY_DOUBLE; };
template <> struct select_npy_type<float> { const static NPY_TYPES type = NPY_FLOAT; };
template <> struct select_npy_type<bool> { const static NPY_TYPES type = NPY_BOOL; };
template <> struct select_npy_type<int8_t> { const static NPY_TYPES type = NPY_INT8; };
template <> struct select_npy_type<int16_t> { const static NPY_TYPES type = NPY_SHORT; };
template <> struct select_npy_type<int32_t> { const static NPY_TYPES type = NPY_INT; };
template <> struct select_npy_type<int64_t> { const static NPY_TYPES type = NPY_INT64; };
template <> struct select_npy_type<uint8_t> { const static NPY_TYPES type = NPY_UINT8; };
template <> struct select_npy_type<uint16_t> { const static NPY_TYPES type = NPY_USHORT; };
template <> struct select_npy_type<uint32_t> { const static NPY_TYPES type = NPY_ULONG; };
template <> struct select_npy_type<uint64_t> { const static NPY_TYPES type = NPY_UINT64; };

template<typename Numeric>
PyObject* get_array(const std::vector<Numeric>& v)
{
    detail::_interpreter::get();    //interpreter needs to be initialized for the numpy commands to work
    NPY_TYPES type = select_npy_type<Numeric>::type;
    if (type == NPY_NOTYPE)
    {
        std::vector<double> vd(v.size());
        npy_intp vsize = v.size();
        std::copy(v.begin(),v.end(),vd.begin());
        PyObject* varray = PyArray_SimpleNewFromData(1, &vsize, NPY_DOUBLE, (void*)(vd.data()));
        return varray;
    }

    npy_intp vsize = v.size();
    PyObject* varray = PyArray_SimpleNewFromData(1, &vsize, type, (void*)(v.data()));
    return varray;
}

#else // fallback if we don't have numpy: copy every element of the given vector

template<typename Numeric>
PyObject* get_array(const std::vector<Numeric>& v)
{
    PyObject* list = PyList_New(v.size());
    for(size_t i = 0; i < v.size(); ++i) {
        PyList_SetItem(list, i, PyFloat_FromDouble(v.at(i)));
    }
    return list;
}

#endif // WITHOUT_NUMPY

template<typename Numeric>
bool plot(const std::vector<Numeric> &x, const std::vector<Numeric> &y, const std::map<std::string, std::string>& keywords)
{
    assert(x.size() == y.size());

    // using numpy arrays
    PyObject* xarray = get_array(x);
    PyObject* yarray = get_array(y);

    // construct positional args
    PyObject* args = PyTuple_New(2);
    PyTuple_SetItem(args, 0, xarray);
    PyTuple_SetItem(args, 1, yarray);

    // construct keyword args
    PyObject* kwargs = PyDict_New();
    for(std::map<std::string, std::string>::const_iterator it = keywords.begin(); it != keywords.end(); ++it)
    {
        PyDict_SetItemString(kwargs, it->first.c_str(), PyString_FromString(it->second.c_str()));
    }

    PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_plot, args, kwargs);

    Py_DECREF(args);
    Py_DECREF(kwargs);
    if(res) Py_DECREF(res);

    return res;
}

template<typename Numeric>
bool stem(const std::vector<Numeric> &x, const std::vector<Numeric> &y, const std::map<std::string, std::string>& keywords)
{
    assert(x.size() == y.size());

    // using numpy arrays
    PyObject* xarray = get_array(x);
    PyObject* yarray = get_array(y);

    // construct positional args
    PyObject* args = PyTuple_New(2);
    PyTuple_SetItem(args, 0, xarray);
    PyTuple_SetItem(args, 1, yarray);

    // construct keyword args
    PyObject* kwargs = PyDict_New();
    for (std::map<std::string, std::string>::const_iterator it =
            keywords.begin(); it != keywords.end(); ++it) {
        PyDict_SetItemString(kwargs, it->first.c_str(),
                PyString_FromString(it->second.c_str()));
    }

    PyObject* res = PyObject_Call(
            detail::_interpreter::get().s_python_function_stem, args, kwargs);

    Py_DECREF(args);
    Py_DECREF(kwargs);
    if (res)
        Py_DECREF(res);

    return res;
}

template< typename Numeric >
bool fill_between(const std::vector<Numeric>& x, const std::vector<Numeric>& y1, const std::vector<Numeric>& y2, const std::map<std::string, std::string>& keywords)
{
    assert(x.size() == y1.size());
    assert(x.size() == y2.size());

    // using numpy arrays
    PyObject* xarray = get_array(x);
    PyObject* y1array = get_array(y1);
    PyObject* y2array = get_array(y2);

    // construct positional args
    PyObject* args = PyTuple_New(3);
    PyTuple_SetItem(args, 0, xarray);
    PyTuple_SetItem(args, 1, y1array);
    PyTuple_SetItem(args, 2, y2array);

    // construct keyword args
    PyObject* kwargs = PyDict_New();
    for(std::map<std::string, std::string>::const_iterator it = keywords.begin(); it != keywords.end(); ++it)
    {
        PyDict_SetItemString(kwargs, it->first.c_str(), PyUnicode_FromString(it->second.c_str()));
    }

    PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_fill_between, args, kwargs);

    Py_DECREF(args);
    Py_DECREF(kwargs);
    if(res) Py_DECREF(res);

    return res;
}

template< typename Numeric>
bool hist(const std::vector<Numeric>& y, long bins=10,std::string color="b", double alpha=1.0)
{

    PyObject* yarray = get_array(y);

    PyObject* kwargs = PyDict_New();
    PyDict_SetItemString(kwargs, "bins", PyLong_FromLong(bins));
    PyDict_SetItemString(kwargs, "color", PyString_FromString(color.c_str()));
    PyDict_SetItemString(kwargs, "alpha", PyFloat_FromDouble(alpha));


    PyObject* plot_args = PyTuple_New(1);

    PyTuple_SetItem(plot_args, 0, yarray);


    PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_hist, plot_args, kwargs);


    Py_DECREF(plot_args);
    Py_DECREF(kwargs);
    if(res) Py_DECREF(res);

    return res;
}

template< typename Numeric>
bool named_hist(std::string label,const std::vector<Numeric>& y, long bins=10, std::string color="b", double alpha=1.0)
{
    PyObject* yarray = get_array(y);

    PyObject* kwargs = PyDict_New();
    PyDict_SetItemString(kwargs, "label", PyString_FromString(label.c_str()));
    PyDict_SetItemString(kwargs, "bins", PyLong_FromLong(bins));
    PyDict_SetItemString(kwargs, "color", PyString_FromString(color.c_str()));
    PyDict_SetItemString(kwargs, "alpha", PyFloat_FromDouble(alpha));


    PyObject* plot_args = PyTuple_New(1);
    PyTuple_SetItem(plot_args, 0, yarray);

    PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_hist, plot_args, kwargs);

    Py_DECREF(plot_args);
    Py_DECREF(kwargs);
    if(res) Py_DECREF(res);

    return res;
}

template<typename NumericX, typename NumericY>
bool plot(const std::vector<NumericX>& x, const std::vector<NumericY>& y, const std::string& s = "")
{
    assert(x.size() == y.size());

    PyObject* xarray = get_array(x);
    PyObject* yarray = get_array(y);

    PyObject* pystring = PyString_FromString(s.c_str());

    PyObject* plot_args = PyTuple_New(3);
    PyTuple_SetItem(plot_args, 0, xarray);
    PyTuple_SetItem(plot_args, 1, yarray);
    PyTuple_SetItem(plot_args, 2, pystring);

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_plot, plot_args);

    Py_DECREF(plot_args);
    if(res) Py_DECREF(res);

    return res;
}

template<typename NumericX, typename NumericY, typename NumericU, typename NumericW>
bool quiver(const std::vector<NumericX>& x, const std::vector<NumericY>& y, const std::vector<NumericU>& u, const std::vector<NumericW>& w, const std::map<std::string, std::string>& keywords = {})
{
    assert(x.size() == y.size() && x.size() == u.size() && u.size() == w.size());

    PyObject* xarray = get_array(x);
    PyObject* yarray = get_array(y);
    PyObject* uarray = get_array(u);
    PyObject* warray = get_array(w);

    PyObject* plot_args = PyTuple_New(4);
    PyTuple_SetItem(plot_args, 0, xarray);
    PyTuple_SetItem(plot_args, 1, yarray);
    PyTuple_SetItem(plot_args, 2, uarray);
    PyTuple_SetItem(plot_args, 3, warray);

    // construct keyword args
    PyObject* kwargs = PyDict_New();
    for(std::map<std::string, std::string>::const_iterator it = keywords.begin(); it != keywords.end(); ++it)
    {
        PyDict_SetItemString(kwargs, it->first.c_str(), PyUnicode_FromString(it->second.c_str()));
    }

    PyObject* res = PyObject_Call(
            detail::_interpreter::get().s_python_function_quiver, plot_args, kwargs);

    Py_DECREF(kwargs);
    Py_DECREF(plot_args);
    if (res)
        Py_DECREF(res);

    return res;
}

template<typename NumericX, typename NumericY>
bool stem(const std::vector<NumericX>& x, const std::vector<NumericY>& y, const std::string& s = "")
{
    assert(x.size() == y.size());

    PyObject* xarray = get_array(x);
    PyObject* yarray = get_array(y);

    PyObject* pystring = PyString_FromString(s.c_str());

    PyObject* plot_args = PyTuple_New(3);
    PyTuple_SetItem(plot_args, 0, xarray);
    PyTuple_SetItem(plot_args, 1, yarray);
    PyTuple_SetItem(plot_args, 2, pystring);

    PyObject* res = PyObject_CallObject(
            detail::_interpreter::get().s_python_function_stem, plot_args);

    Py_DECREF(plot_args);
    if (res)
        Py_DECREF(res);

    return res;
}

template<typename NumericX, typename NumericY>
bool semilogx(const std::vector<NumericX>& x, const std::vector<NumericY>& y, const std::string& s = "")
{
    assert(x.size() == y.size());

    PyObject* xarray = get_array(x);
    PyObject* yarray = get_array(y);

    PyObject* pystring = PyString_FromString(s.c_str());

    PyObject* plot_args = PyTuple_New(3);
    PyTuple_SetItem(plot_args, 0, xarray);
    PyTuple_SetItem(plot_args, 1, yarray);
    PyTuple_SetItem(plot_args, 2, pystring);

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_semilogx, plot_args);

    Py_DECREF(plot_args);
    if(res) Py_DECREF(res);

    return res;
}

template<typename NumericX, typename NumericY>
bool semilogy(const std::vector<NumericX>& x, const std::vector<NumericY>& y, const std::string& s = "")
{
    assert(x.size() == y.size());

    PyObject* xarray = get_array(x);
    PyObject* yarray = get_array(y);

    PyObject* pystring = PyString_FromString(s.c_str());

    PyObject* plot_args = PyTuple_New(3);
    PyTuple_SetItem(plot_args, 0, xarray);
    PyTuple_SetItem(plot_args, 1, yarray);
    PyTuple_SetItem(plot_args, 2, pystring);

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_semilogy, plot_args);

    Py_DECREF(plot_args);
    if(res) Py_DECREF(res);

    return res;
}

template<typename NumericX, typename NumericY>
bool loglog(const std::vector<NumericX>& x, const std::vector<NumericY>& y, const std::string& s = "")
{
    assert(x.size() == y.size());

    PyObject* xarray = get_array(x);
    PyObject* yarray = get_array(y);

    PyObject* pystring = PyString_FromString(s.c_str());

    PyObject* plot_args = PyTuple_New(3);
    PyTuple_SetItem(plot_args, 0, xarray);
    PyTuple_SetItem(plot_args, 1, yarray);
    PyTuple_SetItem(plot_args, 2, pystring);

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_loglog, plot_args);

    Py_DECREF(plot_args);
    if(res) Py_DECREF(res);

    return res;
}

template<typename NumericX, typename NumericY>
bool errorbar(const std::vector<NumericX> &x, const std::vector<NumericY> &y, const std::vector<NumericX> &yerr, const std::map<std::string, std::string> &keywords = {})
{
    assert(x.size() == y.size());

    PyObject* xarray = get_array(x);
    PyObject* yarray = get_array(y);
    PyObject* yerrarray = get_array(yerr);

    // construct keyword args
    PyObject* kwargs = PyDict_New();
    for(std::map<std::string, std::string>::const_iterator it = keywords.begin(); it != keywords.end(); ++it)
    {
        PyDict_SetItemString(kwargs, it->first.c_str(), PyString_FromString(it->second.c_str()));
    }

    PyDict_SetItemString(kwargs, "yerr", yerrarray);

    PyObject *plot_args = PyTuple_New(2);
    PyTuple_SetItem(plot_args, 0, xarray);
    PyTuple_SetItem(plot_args, 1, yarray);

    PyObject *res = PyObject_Call(detail::_interpreter::get().s_python_function_errorbar, plot_args, kwargs);

    Py_DECREF(kwargs);
    Py_DECREF(plot_args);

    if (res)
        Py_DECREF(res);
    else
        throw std::runtime_error("Call to errorbar() failed.");

    return res;
}

template<typename Numeric>
bool named_plot(const std::string& name, const std::vector<Numeric>& y, const std::string& format = "")
{
    PyObject* kwargs = PyDict_New();
    PyDict_SetItemString(kwargs, "label", PyString_FromString(name.c_str()));

    PyObject* yarray = get_array(y);

    PyObject* pystring = PyString_FromString(format.c_str());

    PyObject* plot_args = PyTuple_New(2);

    PyTuple_SetItem(plot_args, 0, yarray);
    PyTuple_SetItem(plot_args, 1, pystring);

    PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_plot, plot_args, kwargs);

    Py_DECREF(kwargs);
    Py_DECREF(plot_args);
    if (res) Py_DECREF(res);

    return res;
}

template<typename Numeric>
bool named_plot(const std::string& name, const std::vector<Numeric>& x, const std::vector<Numeric>& y, const std::string& format = "")
{
    PyObject* kwargs = PyDict_New();
    PyDict_SetItemString(kwargs, "label", PyString_FromString(name.c_str()));

    PyObject* xarray = get_array(x);
    PyObject* yarray = get_array(y);

    PyObject* pystring = PyString_FromString(format.c_str());

    PyObject* plot_args = PyTuple_New(3);
    PyTuple_SetItem(plot_args, 0, xarray);
    PyTuple_SetItem(plot_args, 1, yarray);
    PyTuple_SetItem(plot_args, 2, pystring);

    PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_plot, plot_args, kwargs);

    Py_DECREF(kwargs);
    Py_DECREF(plot_args);
    if (res) Py_DECREF(res);

    return res;
}

template<typename Numeric>
bool named_semilogx(const std::string& name, const std::vector<Numeric>& x, const std::vector<Numeric>& y, const std::string& format = "")
{
    PyObject* kwargs = PyDict_New();
    PyDict_SetItemString(kwargs, "label", PyString_FromString(name.c_str()));

    PyObject* xarray = get_array(x);
    PyObject* yarray = get_array(y);

    PyObject* pystring = PyString_FromString(format.c_str());

    PyObject* plot_args = PyTuple_New(3);
    PyTuple_SetItem(plot_args, 0, xarray);
    PyTuple_SetItem(plot_args, 1, yarray);
    PyTuple_SetItem(plot_args, 2, pystring);

    PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_semilogx, plot_args, kwargs);

    Py_DECREF(kwargs);
    Py_DECREF(plot_args);
    if (res) Py_DECREF(res);

    return res;
}

template<typename Numeric>
bool named_semilogy(const std::string& name, const std::vector<Numeric>& x, const std::vector<Numeric>& y, const std::string& format = "")
{
    PyObject* kwargs = PyDict_New();
    PyDict_SetItemString(kwargs, "label", PyString_FromString(name.c_str()));

    PyObject* xarray = get_array(x);
    PyObject* yarray = get_array(y);

    PyObject* pystring = PyString_FromString(format.c_str());

    PyObject* plot_args = PyTuple_New(3);
    PyTuple_SetItem(plot_args, 0, xarray);
    PyTuple_SetItem(plot_args, 1, yarray);
    PyTuple_SetItem(plot_args, 2, pystring);

    PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_semilogy, plot_args, kwargs);

    Py_DECREF(kwargs);
    Py_DECREF(plot_args);
    if (res) Py_DECREF(res);

    return res;
}

template<typename Numeric>
bool named_loglog(const std::string& name, const std::vector<Numeric>& x, const std::vector<Numeric>& y, const std::string& format = "")
{
    PyObject* kwargs = PyDict_New();
    PyDict_SetItemString(kwargs, "label", PyString_FromString(name.c_str()));

    PyObject* xarray = get_array(x);
    PyObject* yarray = get_array(y);

    PyObject* pystring = PyString_FromString(format.c_str());

    PyObject* plot_args = PyTuple_New(3);
    PyTuple_SetItem(plot_args, 0, xarray);
    PyTuple_SetItem(plot_args, 1, yarray);
    PyTuple_SetItem(plot_args, 2, pystring);

    PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_loglog, plot_args, kwargs);

    Py_DECREF(kwargs);
    Py_DECREF(plot_args);
    if (res) Py_DECREF(res);

    return res;
}

template<typename Numeric>
bool plot(const std::vector<Numeric>& y, const std::string& format = "")
{
    std::vector<Numeric> x(y.size());
    for(size_t i=0; i<x.size(); ++i) x.at(i) = i;
    return plot(x,y,format);
}

template<typename Numeric>
bool stem(const std::vector<Numeric>& y, const std::string& format = "")
{
    std::vector<Numeric> x(y.size());
    for (size_t i = 0; i < x.size(); ++i) x.at(i) = i;
    return stem(x, y, format);
}

template<typename Numeric>
void text(Numeric x, Numeric y, const std::string& s = "")
{
    PyObject* args = PyTuple_New(3);
    PyTuple_SetItem(args, 0, PyFloat_FromDouble(x));
    PyTuple_SetItem(args, 1, PyFloat_FromDouble(y));
    PyTuple_SetItem(args, 2, PyString_FromString(s.c_str()));

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_text, args);
    if(!res) throw std::runtime_error("Call to text() failed.");

    Py_DECREF(args);
    Py_DECREF(res);
}


inline void figure()
{
    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_figure, detail::_interpreter::get().s_python_empty_tuple);
    if(!res) throw std::runtime_error("Call to figure() failed.");

    Py_DECREF(res);
}

inline void figure_size(size_t w, size_t h)
{
    const size_t dpi = 100;
    PyObject* size = PyTuple_New(2);
    PyTuple_SetItem(size, 0, PyFloat_FromDouble((double)w / dpi));
    PyTuple_SetItem(size, 1, PyFloat_FromDouble((double)h / dpi));

    PyObject* kwargs = PyDict_New();
    PyDict_SetItemString(kwargs, "figsize", size);
    PyDict_SetItemString(kwargs, "dpi", PyLong_FromSize_t(dpi));

    PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_figure,
            detail::_interpreter::get().s_python_empty_tuple, kwargs);

    Py_DECREF(kwargs);

    if(!res) throw std::runtime_error("Call to figure_size() failed.");
    Py_DECREF(res);
}

inline void legend()
{
    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_legend, detail::_interpreter::get().s_python_empty_tuple);
    if(!res) throw std::runtime_error("Call to legend() failed.");

    Py_DECREF(res);
}

template<typename Numeric>
void ylim(Numeric left, Numeric right)
{
    PyObject* list = PyList_New(2);
    PyList_SetItem(list, 0, PyFloat_FromDouble(left));
    PyList_SetItem(list, 1, PyFloat_FromDouble(right));

    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, list);

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_ylim, args);
    if(!res) throw std::runtime_error("Call to ylim() failed.");

    Py_DECREF(args);
    Py_DECREF(res);
}

template<typename Numeric>
void xlim(Numeric left, Numeric right)
{
    PyObject* list = PyList_New(2);
    PyList_SetItem(list, 0, PyFloat_FromDouble(left));
    PyList_SetItem(list, 1, PyFloat_FromDouble(right));

    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, list);

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_xlim, args);
    if(!res) throw std::runtime_error("Call to xlim() failed.");

    Py_DECREF(args);
    Py_DECREF(res);
}


inline double* xlim()
{
    PyObject* args = PyTuple_New(0);
    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_xlim, args);
    PyObject* left = PyTuple_GetItem(res,0);
    PyObject* right = PyTuple_GetItem(res,1);

    double* arr = new double[2];
    arr[0] = PyFloat_AsDouble(left);
    arr[1] = PyFloat_AsDouble(right);

    if(!res) throw std::runtime_error("Call to xlim() failed.");

    Py_DECREF(res);
    return arr;
}


inline double* ylim()
{
    PyObject* args = PyTuple_New(0);
    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_ylim, args);
    PyObject* left = PyTuple_GetItem(res,0);
    PyObject* right = PyTuple_GetItem(res,1);

    double* arr = new double[2];
    arr[0] = PyFloat_AsDouble(left);
    arr[1] = PyFloat_AsDouble(right);

    if(!res) throw std::runtime_error("Call to ylim() failed.");

    Py_DECREF(res);
    return arr;
}

template<typename Numeric>
inline void xticks(const std::vector<Numeric> &ticks, const std::vector<std::string> &labels = {}, const std::map<std::string, std::string>& keywords = {})
{
    assert(labels.size() == 0 || ticks.size() == labels.size());

    // using numpy array
    PyObject* ticksarray = get_array(ticks);

    PyObject* args;
    if(labels.size() == 0) {
        // construct positional args
        args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, ticksarray);
    } else {
        // make tuple of tick labels
        PyObject* labelstuple = PyTuple_New(labels.size());
        for (size_t i = 0; i < labels.size(); i++)
            PyTuple_SetItem(labelstuple, i, PyUnicode_FromString(labels[i].c_str()));

        // construct positional args
        args = PyTuple_New(2);
        PyTuple_SetItem(args, 0, ticksarray);
        PyTuple_SetItem(args, 1, labelstuple);
    }

    // construct keyword args
    PyObject* kwargs = PyDict_New();
    for(std::map<std::string, std::string>::const_iterator it = keywords.begin(); it != keywords.end(); ++it)
    {
        PyDict_SetItemString(kwargs, it->first.c_str(), PyString_FromString(it->second.c_str()));
    }

    PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_xticks, args, kwargs);

    Py_DECREF(args);
    Py_DECREF(kwargs);
    if(!res) throw std::runtime_error("Call to xticks() failed");

    Py_DECREF(res);
}

template<typename Numeric>
inline void xticks(const std::vector<Numeric> &ticks, const std::map<std::string, std::string>& keywords)
{
    xticks(ticks, {}, keywords);
}

template<typename Numeric>
inline void yticks(const std::vector<Numeric> &ticks, const std::vector<std::string> &labels = {}, const std::map<std::string, std::string>& keywords = {})
{
    assert(labels.size() == 0 || ticks.size() == labels.size());

    // using numpy array
    PyObject* ticksarray = get_array(ticks);

    PyObject* args;
    if(labels.size() == 0) {
        // construct positional args
        args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, ticksarray);
    } else {
        // make tuple of tick labels
        PyObject* labelstuple = PyTuple_New(labels.size());
        for (size_t i = 0; i < labels.size(); i++)
            PyTuple_SetItem(labelstuple, i, PyUnicode_FromString(labels[i].c_str()));

        // construct positional args
        args = PyTuple_New(2);
        PyTuple_SetItem(args, 0, ticksarray);
        PyTuple_SetItem(args, 1, labelstuple);
    }

    // construct keyword args
    PyObject* kwargs = PyDict_New();
    for(std::map<std::string, std::string>::const_iterator it = keywords.begin(); it != keywords.end(); ++it)
    {
        PyDict_SetItemString(kwargs, it->first.c_str(), PyString_FromString(it->second.c_str()));
    }

    PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_yticks, args, kwargs);

    Py_DECREF(args);
    Py_DECREF(kwargs);
    if(!res) throw std::runtime_error("Call to yticks() failed");

    Py_DECREF(res);
}

template<typename Numeric>
inline void yticks(const std::vector<Numeric> &ticks, const std::map<std::string, std::string>& keywords)
{
    yticks(ticks, {}, keywords);
}

inline void subplot(long nrows, long ncols, long plot_number)
{
    // construct positional args
    PyObject* args = PyTuple_New(3);
    PyTuple_SetItem(args, 0, PyFloat_FromDouble(nrows));
    PyTuple_SetItem(args, 1, PyFloat_FromDouble(ncols));
    PyTuple_SetItem(args, 2, PyFloat_FromDouble(plot_number));

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_subplot, args);
    if(!res) throw std::runtime_error("Call to subplot() failed.");

    Py_DECREF(args);
    Py_DECREF(res);
}

inline void title(const std::string &titlestr)
{
    PyObject* pytitlestr = PyString_FromString(titlestr.c_str());
    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, pytitlestr);

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_title, args);
    if(!res) throw std::runtime_error("Call to title() failed.");

    Py_DECREF(args);
    Py_DECREF(res);
}

inline void suptitle(const std::string &suptitlestr)
{
    PyObject* pysuptitlestr = PyString_FromString(suptitlestr.c_str());
    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, pysuptitlestr);

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_suptitle, args);
    if(!res) throw std::runtime_error("Call to suptitle() failed.");

    Py_DECREF(args);
    Py_DECREF(res);
}

inline void axis(const std::string &axisstr)
{
    PyObject* str = PyString_FromString(axisstr.c_str());
    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, str);

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_axis, args);
    if(!res) throw std::runtime_error("Call to title() failed.");

    Py_DECREF(args);
    Py_DECREF(res);
}

inline void xlabel(const std::string &str)
{
    PyObject* pystr = PyString_FromString(str.c_str());
    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, pystr);

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_xlabel, args);
    if(!res) throw std::runtime_error("Call to xlabel() failed.");

    Py_DECREF(args);
    Py_DECREF(res);
}

inline void ylabel(const std::string &str)
{
    PyObject* pystr = PyString_FromString(str.c_str());
    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, pystr);

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_ylabel, args);
    if(!res) throw std::runtime_error("Call to ylabel() failed.");

    Py_DECREF(args);
    Py_DECREF(res);
}

inline void grid(bool flag)
{
    PyObject* pyflag = flag ? Py_True : Py_False;
    Py_INCREF(pyflag);

    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, pyflag);

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_grid, args);
    if(!res) throw std::runtime_error("Call to grid() failed.");

    Py_DECREF(args);
    Py_DECREF(res);
}

inline void show(const bool block = true)
{
    PyObject* res;
    if(block)
    {
        res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_show,
                detail::_interpreter::get().s_python_empty_tuple);
    }
    else
    {
        PyObject *kwargs = PyDict_New();
        PyDict_SetItemString(kwargs, "block", Py_False);
        res = PyObject_Call( detail::_interpreter::get().s_python_function_show, detail::_interpreter::get().s_python_empty_tuple, kwargs);
	Py_DECREF(kwargs);
    }


    if (!res) throw std::runtime_error("Call to show() failed.");

    Py_DECREF(res);
}

inline void close()
{
    PyObject* res = PyObject_CallObject(
            detail::_interpreter::get().s_python_function_close,
            detail::_interpreter::get().s_python_empty_tuple);

    if (!res) throw std::runtime_error("Call to close() failed.");

    Py_DECREF(res);
}

inline void xkcd() {
    PyObject* res;
    PyObject *kwargs = PyDict_New();

    res = PyObject_Call(detail::_interpreter::get().s_python_function_xkcd,
            detail::_interpreter::get().s_python_empty_tuple, kwargs);

    Py_DECREF(kwargs);

    if (!res)
        throw std::runtime_error("Call to show() failed.");

    Py_DECREF(res);
}

inline void draw()
{
    PyObject* res = PyObject_CallObject(
        detail::_interpreter::get().s_python_function_draw,
        detail::_interpreter::get().s_python_empty_tuple);

    if (!res) throw std::runtime_error("Call to draw() failed.");

    Py_DECREF(res);
}

template<typename Numeric>
inline void pause(Numeric interval)
{
    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, PyFloat_FromDouble(interval));

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_pause, args);
    if(!res) throw std::runtime_error("Call to pause() failed.");

    Py_DECREF(args);
    Py_DECREF(res);
}

inline void save(const std::string& filename)
{
    PyObject* pyfilename = PyString_FromString(filename.c_str());

    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, pyfilename);

    PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_save, args);
    if (!res) throw std::runtime_error("Call to save() failed.");

    Py_DECREF(args);
    Py_DECREF(res);
}

inline void clf() {
    PyObject *res = PyObject_CallObject(
        detail::_interpreter::get().s_python_function_clf,
        detail::_interpreter::get().s_python_empty_tuple);

    if (!res) throw std::runtime_error("Call to clf() failed.");

    Py_DECREF(res);
}

    inline void ion() {
    PyObject *res = PyObject_CallObject(
        detail::_interpreter::get().s_python_function_ion,
        detail::_interpreter::get().s_python_empty_tuple);

    if (!res) throw std::runtime_error("Call to ion() failed.");

    Py_DECREF(res);
}

inline std::vector<std::array<double, 2>> ginput(const int numClicks = 1, const std::map<std::string, std::string>& keywords = {})
{
    PyObject *args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, PyLong_FromLong(numClicks));

    // construct keyword args
    PyObject* kwargs = PyDict_New();
    for(std::map<std::string, std::string>::const_iterator it = keywords.begin(); it != keywords.end(); ++it)
    {
        PyDict_SetItemString(kwargs, it->first.c_str(), PyUnicode_FromString(it->second.c_str()));
    }

    PyObject* res = PyObject_Call(
        detail::_interpreter::get().s_python_function_ginput, args, kwargs);

    Py_DECREF(kwargs);
    Py_DECREF(args);
    if (!res) throw std::runtime_error("Call to ginput() failed.");

    const size_t len = PyList_Size(res);
    std::vector<std::array<double, 2>> out;
    out.reserve(len);
    for (size_t i = 0; i < len; i++) {
        PyObject *current = PyList_GetItem(res, i);
        std::array<double, 2> position;
        position[0] = PyFloat_AsDouble(PyTuple_GetItem(current, 0));
        position[1] = PyFloat_AsDouble(PyTuple_GetItem(current, 1));
        out.push_back(position);
    }
    Py_DECREF(res);

    return out;
}

// Actually, is there any reason not to call this automatically for every plot?
inline void tight_layout() {
    PyObject *res = PyObject_CallObject(
        detail::_interpreter::get().s_python_function_tight_layout,
        detail::_interpreter::get().s_python_empty_tuple);

    if (!res) throw std::runtime_error("Call to tight_layout() failed.");

    Py_DECREF(res);
}

#if __cplusplus > 199711L || _MSC_VER > 1800
// C++11-exclusive content starts here (variadic plot() and initializer list support)

namespace detail {

template<typename T>
using is_function = typename std::is_function<std::remove_pointer<std::remove_reference<T>>>::type;

template<bool obj, typename T>
struct is_callable_impl;

template<typename T>
struct is_callable_impl<false, T>
{
    typedef is_function<T> type;
}; // a non-object is callable iff it is a function

template<typename T>
struct is_callable_impl<true, T>
{
    struct Fallback { void operator()(); };
    struct Derived : T, Fallback { };

    template<typename U, U> struct Check;

    template<typename U>
    static std::true_type test( ... ); // use a variadic function to make sure (1) it accepts everything and (2) its always the worst match

    template<typename U>
    static std::false_type test( Check<void(Fallback::*)(), &U::operator()>* );

public:
    typedef decltype(test<Derived>(nullptr)) type;
    typedef decltype(&Fallback::operator()) dtype;
    static constexpr bool value = type::value;
}; // an object is callable iff it defines operator()

template<typename T>
struct is_callable
{
    // dispatch to is_callable_impl<true, T> or is_callable_impl<false, T> depending on whether T is of class type or not
    typedef typename is_callable_impl<std::is_class<T>::value, T>::type type;
};

template<typename IsYDataCallable>
struct plot_impl { };

template<>
struct plot_impl<std::false_type>
{
    template<typename IterableX, typename IterableY>
    bool operator()(const IterableX& x, const IterableY& y, const std::string& format)
    {
        // 2-phase lookup for distance, begin, end
        using std::distance;
        using std::begin;
        using std::end;

        auto xs = distance(begin(x), end(x));
        auto ys = distance(begin(y), end(y));
        assert(xs == ys && "x and y data must have the same number of elements!");

        PyObject* xlist = PyList_New(xs);
        PyObject* ylist = PyList_New(ys);
        PyObject* pystring = PyString_FromString(format.c_str());

        auto itx = begin(x), ity = begin(y);
        for(size_t i = 0; i < xs; ++i) {
            PyList_SetItem(xlist, i, PyFloat_FromDouble(*itx++));
            PyList_SetItem(ylist, i, PyFloat_FromDouble(*ity++));
        }

        PyObject* plot_args = PyTuple_New(3);
        PyTuple_SetItem(plot_args, 0, xlist);
        PyTuple_SetItem(plot_args, 1, ylist);
        PyTuple_SetItem(plot_args, 2, pystring);

        PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_plot, plot_args);

        Py_DECREF(plot_args);
        if(res) Py_DECREF(res);

        return res;
    }
};

template<>
struct plot_impl<std::true_type>
{
    template<typename Iterable, typename Callable>
    bool operator()(const Iterable& ticks, const Callable& f, const std::string& format)
    {
        if(begin(ticks) == end(ticks)) return true;

        // We could use additional meta-programming to deduce the correct element type of y,
        // but all values have to be convertible to double anyways
        std::vector<double> y;
        for(auto x : ticks) y.push_back(f(x));
        return plot_impl<std::false_type>()(ticks,y,format);
    }
};

} // end namespace detail

// recursion stop for the above
template<typename... Args>
bool plot() { return true; }

template<typename A, typename B, typename... Args>
bool plot(const A& a, const B& b, const std::string& format, Args... args)
{
    return detail::plot_impl<typename detail::is_callable<B>::type>()(a,b,format) && plot(args...);
}

/*
 * This group of plot() functions is needed to support initializer lists, i.e. calling
 *    plot( {1,2,3,4} )
 */
inline bool plot(const std::vector<double>& x, const std::vector<double>& y, const std::string& format = "") {
    return plot<double,double>(x,y,format);
}

inline bool plot(const std::vector<double>& y, const std::string& format = "") {
    return plot<double>(y,format);
}

inline bool plot(const std::vector<double>& x, const std::vector<double>& y, const std::map<std::string, std::string>& keywords) {
    return plot<double>(x,y,keywords);
}

#endif

} // end namespace matplotlibcpp
