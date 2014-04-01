#include "ViewMain.h"
#include "Document.h"
BEGIN_EVENT_TABLE(ViewMain, wxWindow)
    EVT_PAINT(ViewMain::OnPaint)
END_EVENT_TABLE()



ViewMain::ViewMain(wxWindow *parent, Document *doc): wxWindow(parent, wxID_ANY,wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE), _document(doc)
{
	_editBox = new wxTextCtrl(this, TEXT_Main, "f(S) = opt(d in D){R(S,d) o f(T(S,d))}",wxPoint(5,5), wxSize(300,300), wxTE_MULTILINE | wxTE_RICH, wxDefaultValidator, wxTextCtrlNameStr);
/*	_solutionBox = new wxTextCtrl(this, TEXT_Solution, "TODO",wxPoint(5,5), wxSize(300,300), wxTE_MULTILINE | wxTE_RICH, wxDefaultValidator, wxTextCtrlNameStr);
	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
	hSizer->Add(_editBox,1,wxALL|wxEXPAND);
	hSizer->Add(_solutionBox,1,wxALL|wxEXPAND);
	this->SetSizer(hSizer);*/
}


ViewMain::~ViewMain(void)
{
	delete _editBox;
	//delete _solutionBox;
}
void ViewMain::Observe()
{
	_editBox->LoadFile("results.txt");
	Refresh();
}


void ViewMain::OnPaint(wxPaintEvent &event)
{
	wxString txt(_document->GetDisplayText());
	wxPaintDC dc(this);
	
    wxFont font(12, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	dc.DrawText(txt,wxPoint(100,310));
}