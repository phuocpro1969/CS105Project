#include "ControllerFormGL.h"
#include "resource.h"
#include "Log.h"
using namespace Win;

INT_PTR CALLBACK aboutDialogProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

ControllerFormGL::ControllerFormGL(ModelGL* model, ViewFormGL* view) : model(model), view(view)
{
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_CLOSE
///////////////////////////////////////////////////////////////////////////////
int ControllerFormGL::close()
{
    ::DestroyWindow(handle);                    // close it
    Win::log("Form dialog is destroyed.");
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_CREATE
///////////////////////////////////////////////////////////////////////////////
int ControllerFormGL::create()
{
    // initialize all controls
    view->initControls(handle);
    model->setModelObject(IDC_RADIO1);
    // init the matrices
    model->setViewMatrix(0, 0, 7, 0, 0, 0);
    view->setViewMatrix(0, 0, 7, 0, 0, 0);
    view->clearObjectRotation();
    view->setViewShape(IDC_RADIO6);
    model->setModelShape(IDC_RADIO6);
    model->setBoxRotationOX(false);
    model->setBoxRotationOY(false);
    model->setBoxRotationOZ(false);
    model->setFlagFog(false);
    model->setMove(true);
    return 0;
}

void ControllerFormGL::dr(int id) {
    model->setIsDraw(false);
    model->setMove(false);
    model->setSizeObject(0);
    model->setModelObject(id);
    view->setViewObject(id);
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_COMMAND
///////////////////////////////////////////////////////////////////////////////

int ControllerFormGL::command(int id, int command, LPARAM msg)
{
    switch (id)
    {
    case IDC_BUTTON_VIEW_RESET:
        if (command == BN_CLICKED)
        {
            model->setViewMatrix(0, 0, 7, 0, 0, 0);
            view->setViewMatrix(0, 0, 7, 0, 0, 0);
        }
        break;

    case IDC_BUTTON_MODEL_RESET:
        if (command == BN_CLICKED)
        {
            model->setModelMatrix(0, 0, 0, 0, 0, 0);
            view->setModelMatrix(0, 0, 0, 0, 0, 0);
            view->clearObjectRotation();
            model->setModelObject(model->getModelObject());
            model->setBoxRotationOX(false);
            model->setBoxRotationOY(false);
            model->setBoxRotationOZ(false);
        }
        break;

    case IDC_BUTTON_ABOUT:
        if (command == BN_CLICKED)
        {
            // open About dialog
            ::DialogBox((HINSTANCE)::GetWindowLongPtr(handle, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_ABOUT), handle, aboutDialogProcedure);
        }
        break;
    case IDC_RADIO1:
        dr(id);
        break;
    case IDC_RADIO2:
        dr(id);
        break;
    case IDC_RADIO3:
        dr(id);
        break;
    case IDC_RADIO4:
        dr(id);
        break;
    case IDC_RADIO5:
        dr(id);
        break; 
    case IDC_RADIO6:
        view->setViewShape(id);
        model->setModelShape(id);
        break;
    case IDC_RADIO7:
        view->setViewShape(id);
        model->setModelShape(id);
        break;
    case IDC_RADIO8:
        view->setViewShape(id);
        model->setModelShape(id);
        break;
    case IDC_RADIO9:
        dr(id);
        break;
    case IDC_RADIO10:
        dr(id);
        break;
    case IDC_RADIO11:
        view->setViewShape(id);
        model->setModelShape(id);
        break;
    case IDC_RADIO12:
        view->setViewFog();
        if (model->getFlagFog()) model->setFlagFog(false); else model->setFlagFog(true);
    case IDC_CHECK4:
        if (view->boxRotationOXIsCheck()) 
            model->setBoxRotationOX(true);
        else 
            model->setBoxRotationOX(false);
        break;
    case IDC_CHECK5:
        if (view->boxRotationOYIsCheck()) 
            model->setBoxRotationOY(true);
        else 
            model->setBoxRotationOY(false);
        break;
    case IDC_CHECK6:
        if (view->boxRotationOZIsCheck()) 
            model->setBoxRotationOZ(true);
        else 
            model->setBoxRotationOZ(false);
        break;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle horizontal scroll notification
///////////////////////////////////////////////////////////////////////////////
int ControllerFormGL::hScroll(WPARAM wParam, LPARAM lParam)
{
    // check if the message comming from trackbar
    HWND trackbarHandle = (HWND)lParam;

    int position = HIWORD(wParam);              // current tick mark position
    if (trackbarHandle)
    {
        // get control ID
        int trackbarId = ::GetDlgCtrlID(trackbarHandle);
        view->clearObjectRotation();
        model->setBoxRotationOX(false);
        model->setBoxRotationOY(false);
        model->setBoxRotationOZ(false);
        switch (LOWORD(wParam))
        {
        case TB_THUMBTRACK:     // user dragged the slider
            //Win::log("trackbar: %d", position);
            // NOTE: view will update model component
            view->updateTrackbars(trackbarHandle, position);
            break;

        case TB_THUMBPOSITION:  // by WM_LBUTTONUP
            break;

        case TB_LINEUP:         // by VK_RIGHT, VK_DOWN
            break;

        case TB_LINEDOWN:       // by VK_LEFT, VK_UP
            break;

        case TB_TOP:            // by VK_HOME
            break;

        case TB_BOTTOM:         // by VK_END
            break;

        case TB_PAGEUP:         // by VK_PRIOR (User click the channel to prior.)
            break;

        case TB_PAGEDOWN:       // by VK_NEXT (User click the channel to next.)
            break;

        case TB_ENDTRACK:       // by WM_KEYUP (User release a key.)
            // NOTE: view will update model component
            position = (int)::SendMessage(trackbarHandle, TBM_GETPOS, 0, 0);
            view->updateTrackbars(trackbarHandle, position);
            break;
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_NOTIFY
// The id is not guaranteed to be unique, so use NMHDR.hwndFrom and NMHDR.idFrom.
///////////////////////////////////////////////////////////////////////////////
int ControllerFormGL::notify(int id, LPARAM lParam)
{
    // first cast lParam to NMHDR* to know what the control is
    NMHDR* nmhdr = (NMHDR*)lParam;
    HWND from = nmhdr->hwndFrom;
    NMUPDOWN* nmUpDown = 0;

    switch (nmhdr->code)
    {
        // UpDownBox notifications =========
    case UDN_DELTAPOS:         // the change of position has begun
        // cast again lParam to NMUPDOWN*
        //nmUpDown = (NMUPDOWN*)lParam;
        //return view->changeUpDownPosition(from, nmUpDown->iPos + nmUpDown->iDelta);
        break;

    default:
        break;
    }

    // handled notifications
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_TIMER notification
///////////////////////////////////////////////////////////////////////////////
int ControllerFormGL::timer(WPARAM eventId, LPARAM callback)
{
    
    switch (eventId)
    {
    case IDT_TIMER:
        
        // not needed
        //view->updateMatrices();
        break;
    }

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// dialog procedure for About window
///////////////////////////////////////////////////////////////////////////////
INT_PTR CALLBACK aboutDialogProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        break;

    case WM_CLOSE:
    {
        ::EndDialog(hwnd, 0);
    }
    break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_OK && HIWORD(wParam) == BN_CLICKED)
        {
            ::EndDialog(hwnd, 0);
        }
        break;

    case WM_NOTIFY:
        NMHDR* nmhdr = (NMHDR*)lParam;
        HWND from = nmhdr->hwndFrom;
        if (from == ::GetDlgItem(hwnd, IDC_SYSLINK1) && (nmhdr->code == NM_CLICK || nmhdr->code == NM_RETURN))
        {
            // cast again lParam to NMLINK*
            NMLINK* nmlink = (NMLINK*)lParam;
            ::ShellExecute(0, L"open", nmlink->item.szUrl, 0, 0, SW_SHOW);
        }
        break;
    }

    return false;
}

INT_PTR CALLBACK directoryDialogProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        break;

    case WM_CLOSE:
    {
        ::EndDialog(hwnd, 0);
    }
    break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_OK && HIWORD(wParam) == BN_CLICKED)
        {
            ::EndDialog(hwnd, 0);
        }
        break;

    case WM_NOTIFY:
        NMHDR* nmhdr = (NMHDR*)lParam;
        HWND from = nmhdr->hwndFrom;
        if (from == ::GetDlgItem(hwnd, IDC_SYSLINK1) && (nmhdr->code == NM_CLICK || nmhdr->code == NM_RETURN))
        {
            // cast again lParam to NMLINK*
            NMLINK* nmlink = (NMLINK*)lParam;
            ::ShellExecute(0, L"open", nmlink->item.szUrl, 0, 0, SW_SHOW);
        }
        break;
    }

    return false;
}