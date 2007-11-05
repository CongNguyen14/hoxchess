/////////////////////////////////////////////////////////////////////////////
// Name:            hoxHttpConnection.h
// Program's Name:  Huy's Open Xiangqi
// Created:         10/28/2007
//
// Description:     The HTTP-Connection Thread to help the HTTP player.
/////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_HOX_HTTP_CONNECTION_H_
#define __INCLUDED_HOX_HTTP_CONNECTION_H_

#include <wx/wx.h>
#include "hoxThreadConnection.h"
#include "hoxEnums.h"
#include "hoxTypes.h"

// ----------------------------------------------------------------------------
// hoxHttpConnection
// ----------------------------------------------------------------------------

class hoxHttpConnection : public hoxThreadConnection
{
public:
    hoxHttpConnection( const wxString& sHostname,
                       int             nPort );
    ~hoxHttpConnection();

protected:
    /*******************************************
     * Override the parent's event-handler API
     *******************************************/

    virtual void HandleRequest( hoxRequest* request );

private:
    hoxResult   _SendRequest( const wxString& request, wxString& response );

};


#endif /* __INCLUDED_HOX_HTTP_CONNECTION_H_ */