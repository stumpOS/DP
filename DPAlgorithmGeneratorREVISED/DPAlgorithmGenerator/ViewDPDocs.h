#pragma once
#ifndef VIEWDPDOCS_H_
#define VIEWDPDOCS_H_
#include<vector>
#include<string>
#include<wx/window.h>
#include<wx/wx.h>
#include"DocumentObserver.h"
using namespace std;
class Document;

class ViewDPDocs : public wxWindow, public DocumentObserver
{
public:
	ViewDPDocs(wxWindow *parent, Document *doc);
	~ViewDPDocs(void);
	
	virtual void Observe();
	void OnPaint(wxPaintEvent &event);
	wxDECLARE_EVENT_TABLE();
private:
	
	Document *_document;
};
#endif
