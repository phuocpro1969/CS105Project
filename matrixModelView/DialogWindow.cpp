#include <sstream>
#include <iostream>
#include <cstring>
#include "DialogWindow.h"
#include "procedure.h"
using namespace Win;


DialogWindow::DialogWindow(HINSTANCE hInst, WORD id, HWND hParent, Controller* ctrl) : handle(0), instance(hInst), id(id),
parentHandle(hParent), controller(ctrl),
x(0), y(0)
{
}


DialogWindow::~DialogWindow()
{
}

HWND DialogWindow::create()
{
    handle = ::CreateDialogParam(instance, MAKEINTRESOURCE(id), parentHandle, Win::dialogProcedure, (LPARAM)controller);

    //this->show(SW_SHOWDEFAULT);                     // make it visible

    return handle;
}

void DialogWindow::show(int cmdShow)
{
    ::ShowWindow(handle, cmdShow);
    ::UpdateWindow(handle);
}

