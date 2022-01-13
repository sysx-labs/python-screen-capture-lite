//
// Created by per on 1/11/22.
//
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include "converters/ndarray_converter.h"
#include "../WindowCapture.h"

namespace py = pybind11;


class PyWindowCapture : public WindowCapture{
    using WindowCapture::WindowCapture;

    void onFrame(cv::Mat& frame)  override {
        PYBIND11_OVERRIDE_NAME(void, WindowCapture, "on_frame", onFrame, frame);
    }

    void onMouseChange(int x, int y)  override {
        PYBIND11_OVERRIDE_NAME(void, WindowCapture, "on_mouse_change", onMouseChange, x, y);
    }
};



void init_WindowCapture(py::module& m) {

    py::class_<WindowCapture, PyWindowCapture>(m, "WindowCapture")
            .def(py::init())
            .def(py::init<const std::string&, int, int>())
            .def("on_frame", &WindowCapture::onFrame)
            .def("set_mouse_fps", &WindowCapture::setMouseFPS)
            .def("set_capture_fps", &WindowCapture::setCaptureFPS)
            .def("on_mouse_change", &WindowCapture::onMouseChange)
            .def("get_window_handle", &WindowCapture::getWindowHandle)
            .def("start", &WindowCapture::start);


}