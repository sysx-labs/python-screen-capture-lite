//
// Created by per on 1/11/22.
//

//
// Created by per on 28.01.18.
//
#include <pybind11/pybind11.h>
#include "converters/ndarray_converter.h"
namespace py = pybind11;

void init_WindowCapture(py::module&);


void init_version(py::module& m)
{
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

#define INITS \
    init_version(m);          \
    init_WindowCapture(m);

#define BINDING_INIT                                      \
    m.doc() = "Screen Capture Lite";

#ifndef EMBEDDED
PYBIND11_MODULE(screen_capture_lite, m)
{
    NDArrayConverter::init_numpy(); \

BINDING_INIT
INITS
}
#else
PYBIND11_EMBEDDED_MODULE(screen_capture_lite, m)
{
    BINDING_INIT
    INITS
}
#endif