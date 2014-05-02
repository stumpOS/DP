#include "MainFrame.h"


wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(ID_Open, MainFrame::OnOpen)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
	EVT_MENU(ID_Help, MainFrame::OnHelp)
	EVT_BUTTON(ID_Docs,MainFrame::OnHelp)
	EVT_BUTTON(ID_Submit,MainFrame::OnSubmit)
	EVT_RADIOBOX(ID_RadioBox, MainFrame::OnRadioSelect)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	_document = new Document();
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_Open, "&Open File...\tCtrl-O",
	"open input file");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	wxMenu *menuHelp = new wxMenu;
	//menuHelp->Append(wxID_ABOUT);
	menuHelp->Append(ID_Help, "&Help",
	"Details on the principles of DP");
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append( menuFile, "&File" );
	menuBar->Append( menuHelp, "&Help" );
	SetMenuBar( menuBar );

	wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
		wxArrayString strings;
	strings.Add(wxT("&Linear"));
	strings.Add(wxT("&Nonlinear"));

	_radioBox = new wxRadioBox(this, ID_RadioBox, wxT("Problem Type"),wxPoint(306,1), wxDefaultSize, strings, 1, wxRA_SPECIFY_COLS);

	_viewMain = new ViewMain(this,_document);
	_children.push_back(_viewMain);
	vSizer->Add(_viewMain,1,wxALL|wxEXPAND);
	hSizer->Add(_radioBox,1,wxALL|wxEXPAND);
	_ButtonSubmit = new wxButton(this,ID_Submit,"Generate Algorithm");
	hSizer->Add(_ButtonSubmit,1,wxALL|wxEXPAND);
	_ButtonDocs = new wxButton(this,ID_Docs,"Documentation");
	hSizer->Add(_ButtonDocs,1,wxALL|wxEXPAND);
	vSizer->Add(hSizer,0,wxALL|wxEXPAND);
	this->SetSizer(vSizer);
	this->Layout();
	//vSizer->Fit(this);

	
	CreateStatusBar();
	SetStatusText( "Enter the Dynamic Programming Functional Equation" );
}
MainFrame::~MainFrame()
{
	delete _radioBox;
	delete _document;
	DestroyChildren();
}
void MainFrame::OnSubmit(wxCommandEvent& event)
{
	_document->DP();
	Observe();

	SolutionDlg dlg(this);
	dlg.ShowModal();
}
void MainFrame::Observe()
{
	for(int i=0;i<_children.size();i++)
		_children[i]->Observe();
}
void MainFrame::OnExit(wxCommandEvent& event)
{
	Close();
}
void MainFrame::OnHelp(wxCommandEvent& event)
{
	DPDocsDlg dlg(this);
	dlg.ShowModal();
}
void MainFrame::OnAbout(wxCommandEvent& event)
{
wxMessageBox( "This is a dynamic programming application",
"About DP App", wxOK | wxICON_INFORMATION );
}

void MainFrame::OnOpen(wxCommandEvent& event)
{
  std::string file1 = OpenFile();
  if(_document->GetProblemType()==ProblemType::eLCS)
  {
	  std::string file2 = OpenFile();
	  _document->LoadProblemTwoFiles(file1,file2);
  }
  else
	  _document->LoadProblem(file1);
  Observe();
}
std::string MainFrame::OpenFile()
{
  wxFileDialog dlg(this, "Open File","","","*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if(dlg.ShowModal() != wxID_OK)
	  return "";
  std::string filename = dlg.GetPath().c_str();
  return filename;
}
void MainFrame::OnRadioSelect(wxCommandEvent &event)
{
	int selected = _radioBox->GetSelection();
	switch(selected)
	{
	case 0:
		_document->SetProblemType(ProblemType::eLCS);
		break;
	case 1:
		_document->SetProblemType(ProblemType::eClassPath);
		break;
	default:
		break;
	}
	Observe();
}