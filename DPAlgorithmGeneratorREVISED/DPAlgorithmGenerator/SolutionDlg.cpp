#include "SolutionDlg.h"
#include "MainFrame.h"
BEGIN_EVENT_TABLE(SolutionDlg, wxDialog)
	EVT_BUTTON(ID_Close,SolutionDlg::OnClose)
END_EVENT_TABLE()

SolutionDlg::SolutionDlg(MainFrame *parent):wxDialog(parent, wxID_ANY, L"Results File", wxDefaultPosition, wxSize(500,400), wxBORDER_SIMPLE)
{
	_parent = parent;

  _solutionBox = new wxTextCtrl(this, TEXT_Solution, "",wxPoint(5,5), wxSize(800,200), wxTE_MULTILINE|wxHSCROLL | wxTE_RICH, wxDefaultValidator, wxTextCtrlNameStr);
	wxBoxSizer* vSizer;
	vSizer = new wxBoxSizer(wxVERTICAL);
	vSizer->Add(_solutionBox,1,wxEXPAND|wxALL,0);
	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
	vSizer->Add(hSizer,0,wxALL | wxEXPAND);

	_ButtonClose = new wxButton(this,ID_Close,"Close");
	hSizer->Add(_ButtonClose,1,wxALL|wxEXPAND);
	_ButtonClose->SetDefault();


	this->SetSizer(vSizer);
	this->Layout();
	vSizer->Fit(this);
	wxFont font(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	_solutionBox->SetDefaultStyle(wxTextAttr(*wxBLACK,wxNullColour,font,wxTEXT_ALIGNMENT_DEFAULT));
	
	ifstream result("results.txt");
	
	while(!result.eof())
	{
		string line;
		getline(result,line,'>');
		_solutionBox->AppendText(line);
		_solutionBox->AppendText("\n");
	}
	result.close();

  //_solutionBox->LoadFile("results.txt");
  
  Refresh();
}


SolutionDlg::~SolutionDlg(void)
{
	delete _solutionBox;
}

void SolutionDlg::OnClose(wxCommandEvent& event)
{
	Close();
}
