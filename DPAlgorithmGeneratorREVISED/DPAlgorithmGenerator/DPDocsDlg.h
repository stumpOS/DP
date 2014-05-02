#pragma once
#ifndef DPDOCSDLG_H_
#define DPDOCSDLG_H_
#include <wx/dialog.h>
#include<vector>
class MainFrame;
class DocumentObserver;
class ViewDPDocs;
class ViewDPDocsPanel;

class DPDocsDlg: public wxDialog
{
public:
	DPDocsDlg(MainFrame *parent);
	virtual ~DPDocsDlg(void);
	void Observe();

	void OnNext(wxCommandEvent& event);
	void OnBack(wxCommandEvent& event);
	void OnClose(wxCommandEvent& event);
	enum IDs{
		ID_Close,
		ID_Next,
		ID_Back
	};
	wxDECLARE_EVENT_TABLE();
private:
	wxButton *_ButtonNext;
	wxButton *_ButtonClose;
	wxButton *_ButtonBack;
	ViewDPDocs *_view;
	ViewDPDocsPanel *_panel;
	std::vector<DocumentObserver*> _children;
	MainFrame *_parent;

};
#endif

