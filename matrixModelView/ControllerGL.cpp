#include "ControllerGL.h"
#include "Log.h"

using namespace Win;

///////////////////////////////////////////////////////////////////////////////
// default contructor
///////////////////////////////////////////////////////////////////////////////
ControllerGL::ControllerGL(ModelGL* model, ViewGL* view, ViewFormGL* viewForm) : model(model), view(view), loopFlag(false), viewForm(viewForm)
{
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_CLOSE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::close()
{
    // wait for rendering thread is terminated
    loopFlag = false;
    glThread.join();

    ::DestroyWindow(handle);
    Win::log("OpenGL window is destroyed.");
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_CREATE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::create()
{
    // create a OpenGL rendering context
    if (view->createContext(handle, 32, 24, 8, 8))
    {
        Win::log(L"Created OpenGL rendering context.");
    }
    else
    {
        Win::log(L"[ERROR] Failed to create OpenGL rendering context from ControllerGL::create().");
        return -1;
    }

    // create a thread for OpenGL rendering
    glThread = std::thread(&ControllerGL::runThread, this);
    loopFlag = true;
    Win::log(L"Created a rendering thread for OpenGL.");

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_PAINT
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::paint()
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_COMMAND
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::command(int id, int cmd, LPARAM msg)
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// rendering thread
// initialize OpenGL states and start rendering loop
///////////////////////////////////////////////////////////////////////////////
void ControllerGL::runThread()
{
    // set the current RC in this thread
    ::wglMakeCurrent(view->getDC(), view->getRC());

    // initialize OpenGL states
    model->init();
    Win::log(L"Initialized OpenGL states.");

    bool result = model->initShaders();
    if (result)
        Win::log("GLSL shader objects are initialized.");
    else
        Win::log("[ERROR] Failed to initialize GLSL.");

    // cofigure projection matrix
    RECT rect;
    ::GetClientRect(handle, &rect);
    model->setWindowSize(rect.right, rect.bottom);
    Win::log(L"Initialized OpenGL window size.");

    // rendering loop
    Win::log(L"Entering OpenGL rendering thread...");
    while (loopFlag)
    {
        //std::this_thread::yield();      // yield to other processes or threads
        std::this_thread::sleep_for(std::chrono::milliseconds(30)); // yield to other processes or threads
        
        model->draw();
        view->swapBuffers();
        if (model->boxRotationOXIsCheck() || model->boxRotationOYIsCheck() || model->boxRotationOZIsCheck()) {
            int x = model->getModelX();
            int y = model->getModelY();
            int z = model->getModelZ();
            int rx = model->getModelAngleX();
            int ry = model->getModelAngleY();
            int rz = model->getModelAngleZ();
            if (model->boxRotationOXIsCheck()) {
                rx++;
                if (rx > 180)
                    rx = -179;
            }
            if (model->boxRotationOYIsCheck()) {
                ry++;
                if (ry > 180)
                    ry = -179;
            }
            if (model->boxRotationOZIsCheck()) {
                rz++;
                if (rz > 180)
                    rz = -179;
            }
            model->setModelMatrix(x, y, z, rx, ry, rz);
            viewForm->setModelMatrix(x, y, z, rx, ry, rz);
            view->swapBuffers();
        }
    }

    // close OpenGL Rendering Context (RC)
    model->quit();
    view->closeContext(handle);

    Win::log(L"Exit OpenGL rendering thread.");
}

///////////////////////////////////////////////////////////////////////////////
// handle Left mouse down
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::lButtonDown(WPARAM state, int x, int y)
{
    // update mouse position
    model->setMousePosition(x, y);
    if (!model->getisDraw())
        model->setX1Y1SizeObject(x, y);
    // set focus to receive wm_mousewheel event
    ::SetFocus(handle);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle Left mouse up
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::lButtonUp(WPARAM state, int x, int y)
{
    // update mouse position
    if (!model->getisDraw()) {
        model->setSizeObject(x, y);
        model->setIsDraw(true);
        model->setMove(true);
    }
    else if (model->getMove())
        model->setMousePosition(x, y);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle reft mouse down
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::rButtonDown(WPARAM state, int x, int y)
{
    // update mouse position
    model->setMousePosition(x, y);

    // set focus to receive wm_mousewheel event
    ::SetFocus(handle);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle reft mouse up
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::rButtonUp(WPARAM state, int x, int y)
{
    // update mouse position
    model->setMousePosition(x, y);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSEMOVE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::mouseMove(WPARAM state, int x, int y)
{
    if (state == MK_LBUTTON)
    { 
        if (!model->getisDraw()) {
            model->setSizeObject(x, y);
        }
        else if (model->getMove())
            model->rotateCamera(x, y);
    }
    if (state == MK_RBUTTON)
    {
        model->zoomCamera(y);
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSEWHEEL
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::mouseWheel(int state, int delta, int x, int y)
{
    model->zoomCameraDelta(delta / 120.0f);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_SIZE
///////////////////////////////////////////////////////////////////////////////
int ControllerGL::size(int w, int h, WPARAM wParam)
{
    model->setWindowSize(w, h);
    Win::log(L"Changed OpenGL rendering window size: %dx%d.", w, h);
    return 0;
}

int Win::ControllerGL::timer(WPARAM id, LPARAM lParam)
{
    return 0;
}
