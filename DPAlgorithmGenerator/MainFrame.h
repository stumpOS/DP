// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "DocumentObserver.h"
#include "DPDocsDlg.h"
#include "ViewMain.h"
#include "Document.h"
#include <cstdio>
#include<cstdlib>
#include<vector>

class MainFrame: public wxFrame
{
public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	virtual ~MainFrame();
	enum{  
		ID_Help = 1,
		ID_Submit,
		ID_Docs,
		ID_Open,
		ID_RadioBox
	};
	
	void OnSubmit(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnHelp(wxCommandEvent& event);
	void ParsePages(string pagesFile);
	void OnOpen(wxCommandEvent& event);
	std::string OpenFile();
	Document* GetDocument(){return _document;}
	void Observe();
	void OnRadioSelect(wxCommandEvent &event);
	wxDECLARE_EVENT_TABLE();
private:
	std::vector<DocumentObserver*> _children;
	ViewMain *_viewMain;
	Document *_document;
	wxButton *_ButtonSubmit;
	wxButton *_ButtonDocs;
	wxRadioBox *_radioBox;
	
};

