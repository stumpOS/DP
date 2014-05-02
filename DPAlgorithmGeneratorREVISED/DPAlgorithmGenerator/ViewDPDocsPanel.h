#pragma once
#ifndef VIEWDPDOCSPANEL_H_
#define VIEWDPDOCSPANEL_H_
#include<wx/wx.h>
#include "DPDocsDlg.h"
#include "DocumentObserver.h"

class Document;

class ViewDPDocsPanel:public wxScrolledWindow,public DocumentObserver
{
public:
	ViewDPDocsPanel(DPDocsDlg *parent,Document *doc);
	~ViewDPDocsPanel(void);
	virtual void Observe();
	void OnPaint(wxPaintEvent &event);
	wxDECLARE_EVENT_TABLE();
private:
	Document *_document;
	DPDocsDlg *_parent;
};
#endif

