/////////////////////////////////////////////////////////////////////////////
// Name:            MyFrame.h
// Program's Name:  Huy's Open Xiangqi
// Created:         10/02/2007
//
// Description:     The main Frame for the Client.
/////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_MY_FRAME_H_
#define __INCLUDED_MY_FRAME_H_

#include "wx/wx.h"
#include "wx/laywin.h"   // wxSashLayoutWindow
#include "wx/progdlg.h"
#include "wx/socket.h"

/* Forward declarations */
class hoxNetworkTableInfo;
class hoxTable;

// menu items ids
enum
{
    MDI_QUIT = wxID_EXIT,
    MDI_NEW_WINDOW = 101,

    MDI_OPEN_SERVER,    // Open server
    MDI_CONNECT_SERVER, // Connect to server
    MDI_QUERY_TABLES,   // Query for the list of tables
    MDI_DISCONNECT_SERVER, // Disconnect from server

    MDI_CONNECT_WWW_SERVER,

    MDI_TOGGLE,   // toggle view
    MDI_REFRESH,
    MDI_CHANGE_TITLE,
    MDI_CHANGE_POSITION,
    MDI_CHANGE_SIZE,
    MDI_CHILD_QUIT,
    MDI_ABOUT = wxID_ABOUT
};

/** 
 * Log events
 */
DECLARE_EVENT_TYPE(hoxEVT_FRAME_LOG_MSG, wxID_ANY)

/*
 * Define main (MDI) frame
 */
class MyFrame : public wxMDIParentFrame
{
public:
    MyFrame( wxWindow*          parent, 
             const wxWindowID   id, 
             const wxString&    title,
             const wxPoint&     pos, 
             const wxSize&      size, 
             const long         style );

    ~MyFrame();

    // -----
    void SetupMenu();
    void SetupStatusBar();

    void InitToolBar(wxToolBar* toolBar);

    void OnSize(wxSizeEvent& event);
    void OnSashDrag(wxSashEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnNewWindow(wxCommandEvent& event);

    void OnOpenServer(wxCommandEvent& event);
    void OnConnectServer(wxCommandEvent& event);
    void OnDisconnectServer(wxCommandEvent& event);

    void OnConnectWWWServer(wxCommandEvent& event);

    void OnQuit(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

    void DoJoinTable(const wxString& tableId);
    void DoJoinNewWWWTable(const wxString& tableId);
    void DoJoinExistingWWWTable(const hoxNetworkTableInfo& tableInfo);

    void OnWWWResponse(wxCommandEvent& event);
    void OnMYResponse(wxCommandEvent& event);
    void DoJoinExistingMYTable(const hoxNetworkTableInfo& tableInfo);
    void DoJoinNewMYTable(const wxString& tableId);
    void OnFrameLogMsgEvent( wxCommandEvent &event );

private:
    void _OnWWWResponse_Connect( const wxString& responseStr );
    void _OnWWWResponse_List( const wxString& responseStr );
    void _OnWWWResponse_New( const wxString& responseStr );
    void _OnWWWResponse_Join( const wxString& responseStr );
    void _OnWWWResponse_Leave( const wxString& responseStr );

    void _OnMYResponse_Connect( const wxString& responseStr );
    void _OnMYResponse_List( const wxString& responseStr );
    void _OnMYResponse_Join( const wxString& responseStr );
    void _OnMYResponse_New( const wxString& responseStr );

    hoxTable* _CreateNewTable( const wxString& tableId );

private:
    // Logging.  
    wxSashLayoutWindow* m_logWindow; // To contain the log-text below.
    wxTextCtrl*         m_logText;   // Log window for debugging purpose.

    // the progress dialog which we show while worker thread is running
    wxProgressDialog*   m_dlgProgress;


    DECLARE_EVENT_TABLE()
};


#endif  /* __INCLUDED_MY_FRAME_H_ */