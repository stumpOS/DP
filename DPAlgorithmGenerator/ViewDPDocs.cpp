#include "ViewDPDocs.h"
#include "Document.h"

BEGIN_EVENT_TABLE(ViewDPDocs, wxWindow)
    EVT_PAINT(ViewDPDocs::OnPaint)
END_EVENT_TABLE()

ViewDPDocs::ViewDPDocs(wxWindow *parent,Document *doc) : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(400, 300), wxBORDER_SUNKEN), _document(doc)
{
	SetClientSize(wxSize(200, 150));
	
}


ViewDPDocs::~ViewDPDocs(void)
{
}
void ViewDPDocs::Observe()
{
	Refresh();
}
void ViewDPDocs::OnPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);

    // Create the font
    wxFont font(12, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

    // Y location to draw at
    int y = 0;

    // Ask the system how tall the font is
    wxCoord w, h;
    dc.GetTextExtent(L"TEST", &w, &h);
    wxCoord textHeight = h;
	Page *current = _document->GetPage(_document->GetCurrentPage());
	if(current == NULL)
		return;
	wxString titleString(current->sTitle);
	wxString bodyString(current->sBody);
	dc.DrawText(titleString,wxPoint(5,5));
	dc.DrawText(bodyString,wxPoint(5,5+2*h));
}

