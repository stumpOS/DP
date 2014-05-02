#include "ViewDPDocsPanel.h"
#include "Document.h"

BEGIN_EVENT_TABLE(ViewDPDocsPanel, wxWindow)
    EVT_PAINT(ViewDPDocsPanel::OnPaint)
END_EVENT_TABLE()

ViewDPDocsPanel::ViewDPDocsPanel(DPDocsDlg *parent, Document *doc) :
    wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxBORDER_SIMPLE), _document(doc)
{
	SetMinSize(wxSize(150, 100));
    SetBackgroundColour(wxColour(255, 255, 255));
    SetScrollRate(1, 1);
	_parent = parent;
}


ViewDPDocsPanel::~ViewDPDocsPanel(void)
{
}
void ViewDPDocsPanel::Observe()
{
	Refresh();
}

void ViewDPDocsPanel::OnPaint(wxPaintEvent &event)
{
	int curr = _document->GetCurrentPage();
	wxPaintDC dc(this);
	
    wxFont font(12, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	int y=0;
	for(int i=0;i<_document->GetNumPages();i++)
	{
		y = i*15;
		if(i==curr)
		{
			dc.DrawRectangle(wxCoord(1),wxCoord(y+15),wxCoord(150),wxCoord(15));
		}
		wxString title(_document->GetPage(i)->sTitle);
		dc.DrawText(title,wxPoint(1,y));
	}
}