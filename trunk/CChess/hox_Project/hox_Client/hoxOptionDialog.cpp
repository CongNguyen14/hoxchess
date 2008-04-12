/***************************************************************************
 *  Copyright 2007, 2008 Huy Phan  <huyphan@playxiangqi.com>               *
 *                                                                         * 
 *  This file is part of HOXChess.                                         *
 *                                                                         *
 *  HOXChess is free software: you can redistribute it and/or modify       *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  HOXChess is distributed in the hope that it will be useful,            *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with HOXChess.  If not, see <http://www.gnu.org/licenses/>.      *
 ***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// Name:            hoxOptionDialog.cpp
// Created:         03/30/2008
//
// Description:     The dialog to change the Options of a Table.
/////////////////////////////////////////////////////////////////////////////

#include "hoxOptionDialog.h"
#include "hoxTable.h"
#include "hoxUtil.h"

// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------

enum
{
    HOX_ID_SAVE = 100
};

// ----------------------------------------------------------------------------
// Declare event-handler table
// ----------------------------------------------------------------------------
BEGIN_EVENT_TABLE(hoxOptionDialog, wxDialog)
    EVT_BUTTON(HOX_ID_SAVE, hoxOptionDialog::OnButtonSave)
END_EVENT_TABLE()

//-----------------------------------------------------------------------------
// hoxLoginDialog
//-----------------------------------------------------------------------------


hoxOptionDialog::hoxOptionDialog( wxWindow*           parent, 
                                  wxWindowID          id, 
                                  const wxString&     title,
                                  const hoxTable_SPtr pTable )
        : wxDialog( parent, id, title, wxDefaultPosition, wxDefaultSize, 
		            wxDEFAULT_DIALOG_STYLE )
        , m_selectedCommand( COMMAND_ID_CANCEL )
		, m_pTable( pTable )
{
    const hoxTimeInfo currentTimeInfo = m_pTable->GetInitialTime();

	/* Create a layout. */

    wxBoxSizer* topSizer = new wxBoxSizer( wxVERTICAL );

	/* User-Login. */

	wxBoxSizer* timerSizer = new wxStaticBoxSizer(
		new wxStaticBox(this, wxID_ANY, _T("&Timers")), 
		wxHORIZONTAL );

    m_textCtrlTime_Game = new wxTextCtrl( 
		this, 
		wxID_ANY,
        wxString::Format("%d", currentTimeInfo.nGame / 60), // default value
        wxDefaultPosition,
        wxSize(50, wxDefaultCoord ));

    m_textCtrlTime_Move = new wxTextCtrl( 
		this, 
		wxID_ANY,
        wxString::Format("%d", currentTimeInfo.nMove), // default value
        wxDefaultPosition,
        wxSize(50, wxDefaultCoord ));

    m_textCtrlTime_Free = new wxTextCtrl( 
		this, 
		wxID_ANY,
        wxString::Format("%d", currentTimeInfo.nFree), // default value
        wxDefaultPosition,
        wxSize(50, wxDefaultCoord ));

    timerSizer->Add( 
        new wxStaticText(this, wxID_ANY, _T("Game: \n(minutes)")),
		wxSizerFlags().Align(wxALIGN_LEFT).Border(wxTOP, 10));

    timerSizer->Add( 
		m_textCtrlTime_Game,
        wxSizerFlags().Align(wxALIGN_LEFT).Border(wxTOP, 10));

    timerSizer->AddSpacer(20);
    timerSizer->Add( 
        new wxStaticText(this, wxID_ANY, _T("Move: \n(seconds)")),
		wxSizerFlags().Align(wxALIGN_LEFT).Border(wxTOP, 10));

    timerSizer->Add( 
		m_textCtrlTime_Move,
        wxSizerFlags().Align(wxALIGN_LEFT).Border(wxTOP, 10));

    timerSizer->AddSpacer(20);
    timerSizer->Add( 
		new wxStaticText(this, wxID_ANY, _T("Free: \n(seconds)")),
		wxSizerFlags().Align(wxALIGN_LEFT).Border(wxTOP, 10));

    timerSizer->Add( 
		m_textCtrlTime_Free,
        wxSizerFlags().Align(wxALIGN_LEFT).Border(wxTOP, 10));

    topSizer->Add(
		timerSizer, 
		wxSizerFlags().Border(wxALL, 10).Align(wxALIGN_LEFT).Expand());

    /* Buttons */

    wxBoxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );

    buttonSizer->Add( 
		new wxButton(this, HOX_ID_SAVE, _("&Save Options")),
        0,                // make vertically unstretchable
        wxALIGN_CENTER ); // no border and centre horizontally);

    buttonSizer->AddSpacer(20);

    buttonSizer->Add( 
		new wxButton(this, wxID_CANCEL, _("&Cancel")),
        0,                // make vertically unstretchable
        wxALIGN_CENTER ); // no border and centre horizontally);

    topSizer->Add(
		buttonSizer,
		wxSizerFlags().Border(wxALL, 10).Align(wxALIGN_CENTER));

    SetSizer( topSizer );      // use the sizer for layout
	topSizer->SetSizeHints( this );   // set size hints to honour minimum size
}

void 
hoxOptionDialog::OnButtonSave(wxCommandEvent& event)
{
    const char* FNAME = "hoxOptionDialog::OnButtonSave";
    wxString sValue;

	/* Determine the new Timers. */

    sValue = m_textCtrlTime_Game->GetValue();
    m_newTimeInfo.nGame = ::atoi( sValue.c_str() ) * 60; // ... to seconds
	if ( m_newTimeInfo.nGame <= 0  )
	{
		wxLogError("Game Time [%s] is invalid.", sValue.c_str());
		return;
	}

    sValue = m_textCtrlTime_Move->GetValue();
    m_newTimeInfo.nMove = ::atoi( sValue.c_str() );
	if ( m_newTimeInfo.nMove <= 0  )
	{
		wxLogError("Move Time [%s] is invalid.", sValue.c_str());
		return;
	}

    sValue = m_textCtrlTime_Free->GetValue();
    m_newTimeInfo.nFree = ::atoi( sValue.c_str() );
	if ( m_newTimeInfo.nFree <= 0  )
	{
		wxLogError("Free Time [%s] is invalid.", sValue.c_str());
		return;
	}

    wxLogDebug("%s: Table [%s]: The new Timers = [%s].", FNAME,
        m_pTable->GetId().c_str(),
        hoxUtil::TimeInfoToString(m_newTimeInfo).c_str());

	/* Return the SAVE command. */

    m_selectedCommand = COMMAND_ID_SAVE;

    Close();
}

/************************* END OF FILE ***************************************/