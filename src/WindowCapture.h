//
// Created by per on 1/11/22.
//

#ifndef PYTHON_SCREEN_CAPTURE_LITE_WINDOWCAPTURE_H
#define PYTHON_SCREEN_CAPTURE_LITE_WINDOWCAPTURE_H
#include <memory>
#include <ScreenCapture.h>
#include <opencv2/core.hpp>

//30x30x3

class WindowCapture {
public:


private:
    std::vector<SL::Screen_Capture::Window> filteredWindows;
    bool is_screen;
    int window_x = 0;
    int window_y = 0;
    size_t windowHandle;
    std::shared_ptr<SL::Screen_Capture::ICaptureConfiguration<SL::Screen_Capture::ScreenCaptureCallback>> framegrabber_screen;
    std::shared_ptr<SL::Screen_Capture::ICaptureConfiguration<SL::Screen_Capture::WindowCaptureCallback>> framegrabber_window;
public:
    WindowCapture();
    WindowCapture(const std::string& windowName, int y_offset, int index);

    void setMouseFPS(int ps);
    void setCaptureFPS(int ps);
    virtual void onFrame(cv::Mat& mat);
    virtual void onMouseChange(int x, int y);
    void start();

    [[nodiscard]] size_t getWindowHandle() const;



    /*
     *
    //Setup Screen Capture for all monitors
    auto framgrabber =  SL::Screen_Capture::CreateCaptureConfiguration([]() {
        return SL::Screen_Capture::GetMonitors();
    })->onFrameChanged([&](const SL::Screen_Capture::Image& img, const SL::Screen_Capture::Monitor& monitor) {
        // Når bildet har endret seg

    })->onNewFrame([&](const SL::Screen_Capture::Image& img, const SL::Screen_Capture::Monitor& monitor) {
        {
            std::lock_guard<std::mutex> lk(this->capture->cv_m);
            currentImage = cv::Mat(Height(img), Width(img), CV_8UC4, (char*)StartSrc(img)).clone();
        }
        this->capture->cv.notify_all();
        this->capture->_onFrame();
    }

    })->onMouseChanged([&](const SL::Screen_Capture::Image* img, const SL::Screen_Capture::MousePoint &mousepoint) {
        // Kode kan kjøres

    })->start_capturing();

    framgrabber->setFrameChangeInterval(std::chrono::milliseconds(100));//100 ms
    framgrabber->setMouseChangeInterval(std::chrono::milliseconds(100));//100 ms
     */





    std::chrono::duration<int64_t, std::milli> mouse_interval;
    std::chrono::duration<int64_t, std::milli> capture_interval;
};


#endif //PYTHON_SCREEN_CAPTURE_LITE_WINDOWCAPTURE_H
