#pragma once

#ifndef WIN_CONTROLLER_GL_H
#define WIN_CONTROLLER_GL_H

#include <thread>
#include "Controller.h"
#include "ViewGL.h"
#include "ModelGL.h"
#include "ViewFormGL.h"

namespace Win
{
    class ControllerGL : public Controller
    {
    public:
        ControllerGL(ModelGL* model, ViewGL* view, ViewFormGL* viewForm);
        ~ControllerGL() {};

        int close();                                // close the RC and OpenGL window
        int command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
        int create();                               // create RC for OpenGL window and start new thread for rendering
        int paint();
        int lButtonDown(WPARAM state, int x, int y);
        int lButtonUp(WPARAM state, int x, int y);
        int rButtonDown(WPARAM state, int x, int y);
        int rButtonUp(WPARAM state, int x, int y);
        int mouseMove(WPARAM state, int x, int y);
        int mouseWheel(int state, int delta, int x, int y); // for WM_MOUSEWHEEL:state, delta, x, y
        int size(int w, int h, WPARAM wParam);      // for WM_SIZE: width, height, type(SIZE_MAXIMIZED...)
        int timer(WPARAM id, LPARAM lParam);        // for VM_TIMER

    private:
        void runThread();                           // thread for OpenGL rendering
        ViewFormGL* viewForm;
        ModelGL* model;                             // pointer to model component
        ViewGL* view;                               // pointer to view component
        std::thread glThread;                       // opengl rendering thread object
        volatile bool loopFlag;                     // rendering loop flag
    };
}

#endif