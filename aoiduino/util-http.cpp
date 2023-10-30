/******************************************************************************
**
** Copyright 2009-2023 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#include "util-http.h"
/* Client */
#include <Client.h>

/**
* @namespace AoiUtil
* @brief Aoi utility classes.
 */
namespace AoiUtil
{
// Static variables.
    Client *Http::http = 0;
    /**
     * @fn Http::Http( void )
     *
     * Constructor. Member variables are initialized.
     */
    Http::Http( void )
    {
    }
    /**
     * @fn Http::~Http( void )
     *
     * Destructor. Member variables are deleted.
     */
    Http::~Http( void )
    {
    }
    /**
     * @fn String Http::httpGet( StringList *args )
     *
     * Send HTTP GET to server.
     *
     * @param[in] args Reference to arguments.
     * @return Recieved content.
     */
    String Http::httpGet( StringList *args )
    {
        String s;
        String host;
        int port = 80;
        int timeout = 30 * 1000;

        switch( count(args) )
        {
            case 4:
                timeout = _atoi( 3 ) * 1000;
            case 3:
                port = _atoi( 2 );
            case 2:
                host = _a( 0 );
                if( !http->connect(host.c_str(),port) )
                    return httpGet( 0 );
            // GET
                http->println( "GET "+_a(1)+" HTTP/1.0" );
                http->println( "Host: " + host );
                http->println( "User-Agent: " + String(STR_USER_AGENT) );
                http->println( "Connection: close" );
                http->println();
            // Response
                s = response( timeout );
                s = prettyPrintTo( "value", s );
                break;
            default:
                s = usage( "httpGet host path (port timeout)?" );
                break;
        }

        return s;
    }
    /**
     * @fn String Http::httpPost( StringList *args )
     *
     * Send HTTP POST to server.
     *
     * @param[in] args Reference to arguments.
     * @return Recieved content.
     */
    String Http::httpPost( StringList *args )
    {
        String s, t, type;
        int size = 0;
        String host;
        int port = 80;
        int timeout = 30 * 1000;

        switch( count(args) )
        {
            case 5:
                timeout = _atoi( 4 ) * 1000;
            case 4:
                port = _atoi( 3 );
            case 3:
            // Request body
                t = _a( 2 );
            // POST
                host = _a( 0 );
                if( !http->connect(host.c_str(),port) )
                    return httpPost( 0 );
                http->println( "POST "+_a(1)+" HTTP/1.0" );
                http->println( "Host: " + host );
                http->println( "User-Agent: " + String(STR_USER_AGENT) );

                // Add if necessary
                if( (t.indexOf("{")==0) || (t.indexOf("[")==0) )
                    type = "application/json";
                else 
                    type = "text/plain";

                http->println( "Content-Type: "+type );
                http->println( "Content-Length: "+String(t.length()) );
                http->println( "Connection: close" );
                http->println();
                http->println( t );
            // Response
                s = response( timeout );
                s = prettyPrintTo( "value", s );
                break;
            default:
                s = usage( "httpPost host path text (port timeout)?" );
                break;
        }

        return s;
    }
    /**
     * @fn String Http::requestBodyFooterInPut( const String &boundary )
     *
     * Return request body footer in HTTP PUT.
     *
     * @param[in] boundary Boundary string.
     * @return Request body footer string in HTTP PUT.
     */
    String Http::requestBodyFooterInPut( const String &boundary )
    {
        return "\r\n--" + boundary + "\r\n";
    }
    /**
     * @fn String Http::requestBodyHeaderInPut( const String &value )
     *
     * Return request body header in HTTP PUT.
     *
     * @param[in] boundary Boundary string.
     * @param[in] name Content-Disposition: name attribute string.
     * @param[in] value Putted value.
     * @param[in/out] size Putted value size. If value is file, File size is used.
     * @return Request body header string in HTTP PUT.
     */
    String Http::requestBodyHeaderInPut( const String &boundary, const String &name, const String &value, int *size )
    {
        String s;

        s += "--" + boundary + "\r\n";
        s += "Content-Disposition: form-data; name=\"" + name + "\";";
        s += "\r\n";
        s += "Content-Type: text/plain\r\n";
        *size = value.length();
        s += "\r\n";

        return s;
    }
    /**
     * @fn String Http::response( int timeout = 30*1000 )
     *
     * Get response from HTTP server.
     *
     * @param[in] int timeout Response timeout.
     * @return Recieved content.
     */
    String Http::response( int timeout )
    {
        String s;
        char *buf = new char[ _AOIUTIL_HTTP_BUFFER_SIZE_+1 ];
        int start = ::millis();
        int i = 0;

        while( true )
        {
            if( i=http->available() )
            {
                i = http->read( (uint8_t*)buf, _AOIUTIL_HTTP_BUFFER_SIZE_ );
                *(buf+i) = NULL;
                s += buf;
            }
            if( !http->available() && !http->connected() )
            {
                http->stop();
                break;
            }
        // timeout
            if( timeout<(::millis()-start) )
                break;
        }
        delete [] buf;

        i = s.indexOf( "\r\n\r\n" );
        if( -1<i )
            s = s.substring( i+4 );

        return s;
    }
}
