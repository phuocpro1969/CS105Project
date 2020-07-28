#include "Controller.h"
using namespace Win;

Controller::Controller() : handle(0)
{
}

Controller::~Controller()
{
    ::DestroyWindow(handle);
}
