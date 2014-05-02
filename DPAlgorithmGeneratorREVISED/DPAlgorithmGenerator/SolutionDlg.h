#pragma once
#include<wx/wx.h>
#include<wx/dialog.h>
#include<fstream>
class MainFrame;

class SolutionDlg: public wxDialog
{
public:
	SolutionDlg(MainFrame *parent);
	virtual ~SolutionDlg(void);
	void OnClose(wxCommandEvent& event);
	enum IDs{
		ID_Close,
		TEXT_Solution
	};
	wxDECLARE_EVENT_TABLE();
private:
	wxTextCtrl *_solutionBox;
	wxButton *_ButtonClose;
	MainFrame *_parent;
};

