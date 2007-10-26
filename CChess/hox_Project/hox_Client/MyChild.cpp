/////////////////////////////////////////////////////////////////////////////
// Name:            MyChild.cpp
// Program's Name:  Huy's Open Xiangqi
// Created:         10/20/2007
//
// Description:     The child Window for the Client.
/////////////////////////////////////////////////////////////////////////////

#include "MyChild.h"
#include "MyFrame.h"
#include "MyApp.h"    // To access wxGetApp()
#include "hoxTable.h"
#include "hoxPlayer.h"
#include "hoxTableMgr.h"
#include "hoxPlayerMgr.h"
#if !defined(__WXMSW__)
    #include "icons/chart.xpm"
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------


// Note that MDI_NEW_WINDOW and MDI_ABOUT commands get passed
// to the parent window for processing, so no need to
// duplicate event handlers here.
BEGIN_EVENT_TABLE(MyChild, wxMDIChildFrame)
    EVT_MENU(MDI_CHILD_QUIT, MyChild::OnQuit)
    EVT_MENU(MDI_TOGGLE, MyChild::OnToggle)
    EVT_MENU(MDI_REFRESH, MyChild::OnRefresh)
    EVT_MENU(MDI_CHANGE_SIZE, MyChild::OnChangeSize)

    EVT_MOVE(MyChild::OnMove)
    EVT_CLOSE(MyChild::OnClose)
END_EVENT_TABLE()


// ---------------------------------------------------------------------------
// MyChild
// ---------------------------------------------------------------------------

MyChild::MyChild(wxMDIParentFrame *parent, const wxString& title)
       : wxMDIChildFrame(parent, wxID_ANY, title, wxDefaultPosition, 
                         wxSize(400, 500)/*wxDefaultSize*/,
                         wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE)
        , m_table( NULL )
{
    this->Maximize();

    this->SetupMenu();
    this->SetupStatusBar();

    // Give it an icon
    this->SetIcon(wxICON(chart));

    // this should work for MDI frames as well as for normal ones
    SetSizeHints(100, 100);
}

MyChild::~MyChild()
{
    //const char* FNAME = "MyChild::~MyChild";

    //wxLogDebug(wxString::Format("%s: ENTER."));
    if ( m_table != NULL )
    {
        /* We should send a signal inform all players about this event.
         * In the mean time, use "brute force"!!!
         */

        hoxPlayer* redPlayer = m_table->GetRedPlayer();
        hoxPlayer* blackPlayer = m_table->GetBlackPlayer();

        if ( redPlayer != NULL ) redPlayer->LeaveTable( m_table );
        if ( blackPlayer != NULL ) blackPlayer->LeaveTable( m_table );
        hoxTableMgr::GetInstance()->RemoveTable( m_table );
    }
}

void MyChild::SetupMenu()
{
    /* File menu */

    wxMenu *file_menu = new wxMenu;

    file_menu->Append(MDI_NEW_WINDOW, _T("&New window"));
    file_menu->AppendSeparator();
    file_menu->Append(MDI_OPEN_SERVER, _T("&Open Server\tCtrl-O"), _T("Open server for remote access"));
    file_menu->Append(MDI_CONNECT_SERVER, _T("&Connect Server\tCtrl-C"), _T("Connect to remote server"));
    file_menu->Append(MDI_QUERY_TABLES, _T("&Query Tables\tCtrl-L"), _T("Query for list of tables"));
    file_menu->Append(MDI_DISCONNECT_SERVER, _T("&Disconnect Server\tCtrl-D"), _T("Disconnect from remote server"));
    file_menu->AppendSeparator();
    file_menu->Append(MDI_CHILD_QUIT, _T("&Close child"), _T("Close this window"));
    file_menu->Append(MDI_QUIT, _T("&Exit"));

    /* Child menu */

    wxMenu *option_menu = new wxMenu;

    option_menu->Append(MDI_TOGGLE, _T("&Toggle View\tCtrl-T"));
    option_menu->AppendSeparator();
    option_menu->Append(MDI_REFRESH, _T("&Refresh picture"));
    option_menu->Append(MDI_CHANGE_TITLE, _T("Change title..."));
    option_menu->AppendSeparator();
    option_menu->Append(MDI_CHANGE_POSITION, _T("Move frame\tCtrl-M"));
    option_menu->Append(MDI_CHANGE_SIZE, _T("Resize frame\tCtrl-S"));

    /* Help menu */

    wxMenu *help_menu = new wxMenu;
    help_menu->Append(MDI_ABOUT, _T("&About"));

    /* Setup the main menu bar */

    wxMenuBar *menu_bar = new wxMenuBar;

    menu_bar->Append(file_menu, _T("&File"));
    menu_bar->Append(option_menu, _T("&Child"));
    menu_bar->Append(help_menu, _T("&Help"));

    // Associate the menu bar with the frame
    this->SetMenuBar(menu_bar);

}

void MyChild::SetupStatusBar()
{
    this->CreateStatusBar();
    this->SetStatusText( this->GetTitle() );
}

void MyChild::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MyChild::OnToggle(wxCommandEvent& WXUNUSED(event))
{
    if ( m_table != NULL )
        m_table->ToggleViewSide();
}

void MyChild::OnRefresh(wxCommandEvent& WXUNUSED(event))
{
    //if ( canvas )
    //    canvas->Refresh();
}

void MyChild::OnChangeSize(wxCommandEvent& WXUNUSED(event))
{
    SetClientSize(100, 100);
}

void MyChild::OnMove(wxMoveEvent& event)
{
    // VZ: here everything is totally wrong under MSW, the positions are
    //     different and both wrong (pos2 is off by 2 pixels for me which seems
    //     to be the width of the MDI canvas border)
    //wxPoint pos1 = event.GetPosition(),
    //        pos2 = GetPosition();
    //wxLogStatus(wxT("position from event: (%d, %d), from frame (%d, %d)"),
    //            pos1.x, pos1.y, pos2.x, pos2.y);

    event.Skip();
}

void 
MyChild::OnClose(wxCloseEvent& event)
{
    const char* FNAME = "MyChild::OnClose";

    wxGetApp().m_nChildren--;

    /* If one of the players is the WWW local player,
     * then inform the WWW server that the player is leaving this table.
     */
    wxASSERT( m_table != NULL );
    hoxWWWPlayer* wwwLocalPlayer = wxGetApp().m_wwwLocalPlayer;
    if (   wwwLocalPlayer != NULL
        && (   m_table->GetRedPlayer() == wwwLocalPlayer
            || m_table->GetBlackPlayer() == wwwLocalPlayer) )
    {
        wxLogDebug(wxString::Format("%s: Info the WWW server about my leaving my table.", FNAME));
        const wxString tableId = m_table->GetId(); 
        wxASSERT( this->GetParent() );
        hoxResult result = wwwLocalPlayer->LeaveNetworkTable( tableId, this->GetParent() );
        if ( result != hoxRESULT_OK ) // failed?
        {
            wxLogError(wxString::Format("%s: Failed to inform WWW server about my leaving the table [%s].", FNAME, tableId));
        }
        wwwLocalPlayer->LeaveTable( m_table );
    }

    /* If one of the players is the MY player,
     * then inform the the server that the player is leaving this table.
     */
    wxASSERT( m_table != NULL );
    hoxMyPlayer* myPlayer = wxGetApp().m_myPlayer;
    if (   myPlayer != NULL
        && (   m_table->GetRedPlayer() == myPlayer
            || m_table->GetBlackPlayer() == myPlayer) )
    {
        wxLogDebug(wxString::Format("%s: Info the server about my leaving my table.", FNAME));
        const wxString tableId = m_table->GetId(); 
        wxASSERT( this->GetParent() );
        hoxResult result = myPlayer->LeaveNetworkTable( tableId, this->GetParent() );
        if ( result != hoxRESULT_OK ) // failed?
        {
            wxLogError(wxString::Format("%s: Failed to inform the server about my leaving the table [%s].", FNAME, tableId));
        }
        myPlayer->LeaveTable( m_table );
    }

    event.Skip(); // let the search for the event handler should continue...
}

void
MyChild::SetTable(hoxTable* table)
{
    wxASSERT_MSG( m_table == NULL, "A table has already been set." );
    wxASSERT( table != NULL );
    m_table = table;
}


/************************* END OF FILE ***************************************/