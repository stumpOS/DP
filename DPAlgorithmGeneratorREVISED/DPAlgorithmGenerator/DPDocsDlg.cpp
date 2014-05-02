#include "DPDocsDlg.h"
#include "MainFrame.h"
#include "DocumentObserver.h"
#include "ViewDPDocs.h"
#include "ViewDPDocsPanel.h"

BEGIN_EVENT_TABLE(DPDocsDlg, wxDialog)
	EVT_BUTTON(ID_Close,DPDocsDlg::OnClose)
	EVT_BUTTON(ID_Next,DPDocsDlg::OnNext)
	EVT_BUTTON(ID_Back,DPDocsDlg::OnBack)
END_EVENT_TABLE()

DPDocsDlg::DPDocsDlg(MainFrame *parent):
    wxDialog(parent, wxID_ANY, L"Fundamentals of DP", wxDefaultPosition, wxSize(500,400), wxMAXIMIZE_BOX)
{
	_parent = parent;
	wxBoxSizer* vSizer;
	vSizer = new wxBoxSizer(wxVERTICAL);
	_view = new ViewDPDocs(this,parent->GetDocument());
	_panel = new ViewDPDocsPanel(this,parent->GetDocument());
	_children.push_back(_view);
	_children.push_back(_panel);

	wxBoxSizer* inHSizer = new wxBoxSizer(wxHORIZONTAL);
	inHSizer->Add(_panel,0, wxEXPAND | wxALL,0);
	inHSizer->Add(_view,1,wxEXPAND | wxALL,0);
	vSizer->Add(inHSizer,1,wxALL | wxEXPAND);

	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
	vSizer->Add(hSizer,0,wxALL | wxEXPAND);

	_ButtonClose = new wxButton(this,ID_Close,"Close");
	hSizer->Add(_ButtonClose,1,wxALL|wxEXPAND);

	_ButtonBack = new wxButton(this,ID_Back,"Previous");
	hSizer->Add(_ButtonBack,1,wxALL|wxEXPAND);

	_ButtonNext = new wxButton(this,ID_Next,"Next");
	hSizer->Add(_ButtonNext,1,wxALL|wxEXPAND);

	_ButtonClose->SetDefault();
	this->SetSizer(vSizer);
	this->Layout();
	vSizer->Fit(this);

}


DPDocsDlg::~DPDocsDlg(void)
{
	DestroyChildren();
}
void DPDocsDlg::Observe()
{
	for(int i=0;i<_children.size();i++)
		_children[i]->Observe();
	_parent->Observe();
}
void DPDocsDlg::OnNext(wxCommandEvent& event)
{
	_parent->GetDocument()->Next();
	Observe();
}
void DPDocsDlg::OnBack(wxCommandEvent& event)
{
	_parent->GetDocument()->GoTo(-1);
	Observe();
}
void DPDocsDlg::OnClose(wxCommandEvent& event)
{
	Close();
}