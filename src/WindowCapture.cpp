//
// Created by per on 1/11/22.
//

#include "WindowCapture.h"
#include <ScreenCapture.h>
#include <iostream>
#include <algorithm>
WindowCapture::WindowCapture()
: is_screen(true)
, mouse_interval(100)
, capture_interval(100)
{
    framegrabber_screen = SL::Screen_Capture::CreateCaptureConfiguration([]() {
        return SL::Screen_Capture::GetMonitors();
    });

}




WindowCapture::WindowCapture(const std::string& windowName, int y_offset, int index)
: is_screen(false)
, mouse_interval(100)
, capture_interval(100){
    auto windows = SL::Screen_Capture::GetWindows();

    std::string srchterm = windowName;
    // convert to lower case for easier comparisons
    std::transform(srchterm.begin(), srchterm.end(), srchterm.begin(),
                   [](char c) { return std::tolower(c, std::locale()); });
    for (auto &a: windows) {
        std::string name = a.Name;
        std::transform(name.begin(), name.end(), name.begin(),
                       [](char c) { return std::tolower(c, std::locale()); });
        if (name.find(srchterm) != std::string::npos) {
            filteredWindows.push_back(a);
            // Offset
            SL::Screen_Capture::OffsetY(a, SL::Screen_Capture::OffsetY(a) + y_offset);
        }
    }

    if(filteredWindows.empty()){
        std::cerr << "Could not find Window!" << std::endl;
        std::cerr << "Existing Windows: " << std::endl;
        for(auto&a : windows){
            std::cerr << "\t-" << a.Name << std::endl;
        }
        throw std::runtime_error("Could not find window!");
    }

    auto found_index = filteredWindows.at(index);
    filteredWindows.clear();
    filteredWindows.push_back(found_index);

    windowHandle = filteredWindows.back().Handle;



    framegrabber_window = SL::Screen_Capture::CreateCaptureConfiguration([&]() {
        return filteredWindows;
    });

}

void WindowCapture::onFrame(cv::Mat& mat) {
    std::cout << "Image!" << std::endl;
}

void WindowCapture::onMouseChange(int x, int y) {
    std::cout << "mouse!" << std::endl;
}



void WindowCapture::start() {
    if(is_screen){
        framegrabber_screen->onNewFrame([&](const SL::Screen_Capture::Image& img, const SL::Screen_Capture::Monitor& monitor) {
            auto image = cv::Mat(Height(img), Width(img), CV_8UC4, (char*)StartSrc(img)).clone();
            onFrame(image);
        });
        framegrabber_screen->onMouseChanged([&](const SL::Screen_Capture::Image* img, const SL::Screen_Capture::MousePoint &mousepoint) {
            onMouseChange(mousepoint.Position.x, mousepoint.Position.y);
        });

        auto grabber = framegrabber_screen->start_capturing();
        grabber->setMouseChangeInterval(mouse_interval);
        grabber->setFrameChangeInterval(capture_interval);
    }else{
        framegrabber_window->onNewFrame([&](const SL::Screen_Capture::Image& img, const SL::Screen_Capture::Window& monitor) {
            auto image = cv::Mat(Height(img), Width(img), CV_8UC4, (char*)StartSrc(img)).clone();
            window_x = monitor.Position.x;
            window_y = monitor.Position.y;
            onFrame(image);
        });
        framegrabber_window->onMouseChanged([&](const SL::Screen_Capture::Image* img, const SL::Screen_Capture::MousePoint &mousepoint) {
            onMouseChange(mousepoint.Position.x - window_x, mousepoint.Position.y - window_y);
        });

        auto grabber = framegrabber_window->start_capturing();
        grabber->setMouseChangeInterval(mouse_interval);
        grabber->setFrameChangeInterval(capture_interval);
    }




}

size_t WindowCapture::getWindowHandle() const {
    return windowHandle;
}

void WindowCapture::setMouseFPS(int ps) {
    mouse_interval = std::chrono::milliseconds((int)std::ceil(ps / 1000.0));
}

void WindowCapture::setCaptureFPS(int ps) {
    capture_interval = std::chrono::milliseconds((int)std::ceil(ps / 1000.0));
}
