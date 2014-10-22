#pragma once

#include <vector>
#include <map>
#include <numeric>
#include <stdexcept>
#include <iostream>

#if __cplusplus > 199711L
#include <functional>
#endif

#include <python2.7/Python.h>

namespace matplotlibcpp {

	namespace detail {
		struct _pyplot_global {
			PyObject *s_python_function_show;
			PyObject *s_python_function_figure;
			PyObject *s_python_function_plot;
			PyObject *s_python_function_legend;
			PyObject *s_python_function_xlim;
			PyObject *s_python_function_ylim;
			PyObject *s_python_empty_tuple;

			static _pyplot_global& get() {
				static _pyplot_global ctx;
				return ctx;
			}

			private:
			_pyplot_global() {
				char name[] = "plotting"; // silence compiler warning abount const strings
				Py_SetProgramName(name);  // optional but recommended
				Py_Initialize();

				PyObject* pyname = PyString_FromString("matplotlib.pyplot");
				if(!pyname) { throw std::runtime_error("couldnt create string"); }

				PyObject* pymod = PyImport_Import(pyname);
				Py_DECREF(pyname);
				if(!pymod) { throw std::runtime_error("Error loading module!"); }

				s_python_function_show = PyObject_GetAttrString(pymod, "show");
				s_python_function_figure = PyObject_GetAttrString(pymod, "figure");
				s_python_function_plot = PyObject_GetAttrString(pymod, "plot");
				s_python_function_legend = PyObject_GetAttrString(pymod, "legend");
				s_python_function_ylim = PyObject_GetAttrString(pymod, "ylim");
				s_python_function_xlim = PyObject_GetAttrString(pymod, "xlim");

				if(!s_python_function_show 
						|| !s_python_function_figure 
						|| !s_python_function_plot 
						|| !s_python_function_legend
						|| !s_python_function_xlim
						|| !s_python_function_ylim) 
				{ throw std::runtime_error("Couldnt find required function!"); }

				if(!PyFunction_Check(s_python_function_show)
					|| !PyFunction_Check(s_python_function_figure)
					|| !PyFunction_Check(s_python_function_plot)
					|| !PyFunction_Check(s_python_function_legend)
					|| !PyFunction_Check(s_python_function_xlim)
					|| !PyFunction_Check(s_python_function_ylim)) 
				{ throw std::runtime_error("Python object is unexpectedly not a PyFunction."); }

				s_python_empty_tuple = PyTuple_New(0);
			}

			~_pyplot_global() {
				Py_Finalize();
			}
		};
	}

	

	template<typename Numeric>
	bool plot(const std::vector<Numeric> &x, const std::vector<Numeric> &y, const std::map<std::string, std::string>& keywords)
	{
		assert(x.size() == y.size());

		// using python lists
		PyObject* xlist = PyList_New(x.size());
		PyObject* ylist = PyList_New(y.size());

		for(size_t i = 0; i < x.size(); ++i) {
			PyList_SetItem(xlist, i, PyFloat_FromDouble(x.at(i)));
			PyList_SetItem(ylist, i, PyFloat_FromDouble(y.at(i)));
		}

		// construct positional args
		PyObject* args = PyTuple_New(2);
		PyTuple_SetItem(args, 0, xlist);
		PyTuple_SetItem(args, 1, ylist);

		Py_DECREF(xlist);
		Py_DECREF(ylist);

		// construct keyword args
		PyObject* kwargs = PyDict_New();
		for(std::map<std::string, std::string>::const_iterator it = keywords.begin(); it != keywords.end(); ++it)
		{
			PyDict_SetItemString(kwargs, it->first.c_str(), PyString_FromString(it->second.c_str()));
		}

		PyObject* res = PyObject_Call(detail::_pyplot_global::get().s_python_function_plot, args, kwargs);

		Py_DECREF(args);
		Py_DECREF(kwargs);
		if(res) Py_DECREF(res);

		return res;
	}


	template<typename NumericX, typename NumericY>
	bool plot(const std::vector<NumericX>& x, const std::vector<NumericY>& y, const std::string& s = "")
	{
		assert(x.size() == y.size());

		//std::string format(s);

		PyObject* xlist = PyList_New(x.size());
		PyObject* ylist = PyList_New(y.size());
		PyObject* pystring = PyString_FromString(s.c_str());

		for(size_t i = 0; i < x.size(); ++i) {
			PyList_SetItem(xlist, i, PyFloat_FromDouble(x.at(i)));
			PyList_SetItem(ylist, i, PyFloat_FromDouble(y.at(i)));
		}

		PyObject* plot_args = PyTuple_New(3);
		PyTuple_SetItem(plot_args, 0, xlist);
		PyTuple_SetItem(plot_args, 1, ylist);
		PyTuple_SetItem(plot_args, 2, pystring);

		PyObject* res = PyObject_CallObject(detail::_pyplot_global::get().s_python_function_plot, plot_args);

		Py_DECREF(xlist);
		Py_DECREF(ylist);
		Py_DECREF(plot_args);
		if(res) Py_DECREF(res);

		return res;
	}


	template<typename Numeric>
	bool named_plot(const std::string& name, const std::vector<Numeric>& x, const std::vector<Numeric>& y, const std::string& format = "") {
		PyObject* kwargs = PyDict_New();
		PyDict_SetItemString(kwargs, "label", PyString_FromString(name.c_str()));

		PyObject* xlist = PyList_New(x.size());
		PyObject* ylist = PyList_New(y.size());
		PyObject* pystring = PyString_FromString(format.c_str());

		for(size_t i = 0; i < x.size(); ++i) {
			PyList_SetItem(xlist, i, PyFloat_FromDouble(x.at(i)));
			PyList_SetItem(ylist, i, PyFloat_FromDouble(y.at(i)));
		}

		PyObject* plot_args = PyTuple_New(3);
		PyTuple_SetItem(plot_args, 0, xlist);
		PyTuple_SetItem(plot_args, 1, ylist);
		PyTuple_SetItem(plot_args, 2, pystring);

		PyObject* res = PyObject_Call(detail::_pyplot_global::get().s_python_function_plot, plot_args, kwargs);

		Py_DECREF(kwargs);
		Py_DECREF(xlist);
		Py_DECREF(ylist);
		Py_DECREF(plot_args);
		if(res) Py_DECREF(res);

		return res;
	}

	template<typename Numeric>
	bool plot(const std::vector<Numeric>& y, const std::string& format = "")
	{
		std::vector<Numeric> x(y.size());
		for(size_t i=0; i<x.size(); ++i) x.at(i) = i;
		return plot(x,y,format);
	}

	/*
	 * This group of plot() functions is needed to support initializer lists, i.e. calling
	 *    plot( {1,2,3,4} )
	 */
	bool plot(const std::vector<double>& x, const std::vector<double>& y, const std::string& format = "") {
		return plot<double,double>(x,y,format);
	}

	bool plot(const std::vector<double>& y, const std::string& format = "") {
		return plot<double>(y,format);
	}

	bool plot(const std::vector<double>& x, const std::vector<double>& y, const std::map<std::string, std::string>& keywords) {
		return plot<double>(x,y,keywords);
	}

	bool named_plot(const std::string& name, const std::vector<double>& x, const std::vector<double>& y, const std::string& format = "") {
		return named_plot<double>(name,x,y,format);
	}

#if __cplusplus > 199711L

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
		static std::true_type test( ... ); // use a variadic function to make use (1) it accepts everything and (2) its always the worst match

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
		typedef typename is_callable_impl<std::is_class<T>::value, T>::type type; // todo: restore remove_reference
	};

	template<typename IsYDataCallable>
	struct plot_impl { };

	template<>
	struct plot_impl<std::false_type>
	{
		template<typename IterableX, typename IterableY>
		bool operator()(const IterableX& x, const IterableY& y, const std::string& format)
		{
			// It's annoying that we have to repeat the code of plot() above
			auto xs = std::distance(std::begin(x), std::end(x));
			auto ys = std::distance(std::begin(y), std::end(y));
			assert(xs == ys && "x and y data must have the same number of elements!");

			PyObject* xlist = PyList_New(xs);
			PyObject* ylist = PyList_New(ys);
			PyObject* pystring = PyString_FromString(format.c_str());

			auto itx = std::begin(x), ity = std::begin(y);
			for(size_t i = 0; i < xs; ++i) {
				PyList_SetItem(xlist, i, PyFloat_FromDouble(*itx++));
				PyList_SetItem(ylist, i, PyFloat_FromDouble(*ity++));
			}

			PyObject* plot_args = PyTuple_New(3);
			PyTuple_SetItem(plot_args, 0, xlist);
			PyTuple_SetItem(plot_args, 1, ylist);
			PyTuple_SetItem(plot_args, 2, pystring);

			PyObject* res = PyObject_CallObject(detail::_pyplot_global::get().s_python_function_plot, plot_args);

			Py_DECREF(xlist);
			Py_DECREF(ylist);
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
			//std::cout << "Callable impl called" << std::endl;

			if(begin(ticks) == end(ticks)) return true;

			// We could use additional meta-programming to deduce the correct element type of y, 
			// but all values have to be convertible to double anyways
			std::vector<double> y;
			for(auto x : ticks) y.push_back(f(x)); 
			return plot_impl<std::false_type>()(ticks,y,format);
		}
	};

	// recursion stop for the above
	template<typename... Args>
	bool plot() { return true; }

	template<typename A, typename B, typename... Args>
	bool plot(const A& a, const B& b, const std::string& format, Args... args)
	{
		return plot_impl<typename is_callable<B>::type>()(a,b,format) && plot(args...);
	}

#endif

	inline void legend() {
		PyObject* res = PyObject_CallObject(detail::_pyplot_global::get().s_python_function_legend, detail::_pyplot_global::get().s_python_empty_tuple);
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

		PyObject* res = PyObject_CallObject(detail::_pyplot_global::get().s_python_function_ylim, args);
		if(!res) throw std::runtime_error("Call to ylim() failed.");

		Py_DECREF(list);
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

		PyObject* res = PyObject_CallObject(detail::_pyplot_global::get().s_python_function_xlim, args);
		if(!res) throw std::runtime_error("Call to xlim() failed.");

		Py_DECREF(list);
		Py_DECREF(args);
		Py_DECREF(res);
	}

	inline void show() {
		PyObject* res = PyObject_CallObject(detail::_pyplot_global::get().s_python_function_show, detail::_pyplot_global::get().s_python_empty_tuple);
		if(!res) throw std::runtime_error("Call to show() failed.");

		Py_DECREF(res);
	}


}
