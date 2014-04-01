#include "MainFrame.h"

// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class MyApp: public wxApp
{
public:
virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
MainFrame *frame = new MainFrame( "Dynamic Programming Algorithm Generator", wxPoint(100, 100), wxSize(675, 510) );
frame->Show( true );
return true;
}
