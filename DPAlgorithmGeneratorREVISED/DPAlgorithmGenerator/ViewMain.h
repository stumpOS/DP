#pragma once
#ifndef VIEWMAIN_H_
#define VIEWMAIN_H_
#include <wx/wx.h>
#include "DocumentObserver.h"
class Document;

class ViewMain : public wxWindow, public DocumentObserver
{
public:
	ViewMain(wxWindow *parent, Document *doc);
	~ViewMain(void);
	void OnPaint(wxPaintEvent &event);
	
	virtual void Observe();
	enum{
		TEXT_Main = wxID_HIGHEST + 1,
		TEXT_Solution
	};
	wxDECLARE_EVENT_TABLE();
private:
	Document *_document;
	wxTextCtrl *_editBox;
	//wxTextCtrl *_solutionBox;
	
};
#endif

