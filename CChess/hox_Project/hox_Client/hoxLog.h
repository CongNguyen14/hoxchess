/***************************************************************************
 *  Copyright 2007 Huy Phan  <huyphan@playxiangqi.com>                     *
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
// Name:            hoxLog.h
// Created:         11/18/2007
//
// Description:     The Log for the Application.
/////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_HOX_LOG_H_
#define __INCLUDED_HOX_LOG_H_

#include <wx/wx.h>

/**
 * The Log of the Application.
 * NOTE: Currently, this log is not being used due to its instabilities.
 *       Perhaps, this issue is caused by log messages coming from secondary
 *       threads and wxLogGui is not thread-safe.
 *
 * @note We go through all these troubles because the wxLogGui is
 *       not thread-safe (would crash under multi-threads running).
 */
class hoxLog : public wxLog
{
public:
    hoxLog();
    virtual ~hoxLog();

    /***********************************
     * Override the parent's API.
     ***********************************/

    /**
     * Log a given string with a timestamp.
     */
    virtual void DoLogString( const wxChar* msg, 
                              time_t        timestamp);

private:
    wxString   m_filename;
};


#endif  /* __INCLUDED_HOX_LOG_H_ */