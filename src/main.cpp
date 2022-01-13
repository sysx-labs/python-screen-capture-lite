//
// Created by per on 1/11/22.
//
#include <chrono>
#include <ScreenCapture.h>
#include "WindowCapture.h"


int main(){

    /*
     *
     */

    auto x = WindowCapture("virtual machine manager", 20, 0);
    x.start();

    while(true){}
}