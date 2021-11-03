/******************************************************************************
**
** Copyright 2009-2021 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#include <IRutils.h>
#include "base-arduinojson.h"
#include "util-irremote.h"

/**
* @namespace AoiUtil
* @brief Aoi utility classes.
 */
namespace AoiUtil
{
// Static variables.
    IRrecv *IRRemote::irReceive = 0;
    IRsend *IRRemote::irSend = 0;
    /**
     * @fn IRRemote::IRRemote( void )
     *
     * Constructor. Member variables are initialized.
     */
    IRRemote::IRRemote( void )
    {
    }
    /**
     * @fn IRRemote::~IRRemote( void )
     *
     * Destructor. Member variables are deleted.
     */
    IRRemote::~IRRemote( void )
    {
    }
    /**
     * @fn String IRRemote::irReceiveBegin( StringList *args )
     *
     * Set GPIO pin to use when receiving an IR command.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String IRRemote::irReceiveBegin( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 3:
                if( irReceive )
                    delete irReceive;
                irReceive = new IRrecv( _atoui(0), _atoui(1), _atoui(2), true );
                irReceive->setTolerance();
                irReceive->enableIRIn();
                break;
            default:
                s = usage( "irReceiveBegin pin size timeout" );
                break;
        }

        return s;
    }
    /**
     * @fn String IRRemote::irReceiveRaw( StringList *args )
     *
     * Receive a raw IR message.
     *
     * @param[in] args Reference to arguments.
     * @return Decoded value.
     */
    String IRRemote::irReceiveRaw( StringList *args )
    {
        String s;
        uint16_t c = count( args );
        decode_results result;

        if( c<1 || !irReceive )
            s = usage( "irReceiveRaw timeout" );
        else
        {
            int msec = _atoui( 0 );
            int i = 0;

            while( (i<msec) && !s.length() )
            {
                if( !irReceive->decode(&result) )
                {
                    delay( 1 );
                    i++;
                    continue;
                }

                DynamicJsonBuffer json;
                JsonObject &r = json.createObject();
                String delim = " ";
            // from https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/IRutils.cpp
            // resultToSourceCode
                uint16_t c = getCorrectedRawLength( &result );

                for( uint16_t j=1; j<result.rawlen; j++ )
                {
                    uint32_t usecs;
                    for( usecs = result.rawbuf[j] * 2;
                         usecs > UINT16_MAX;
                         usecs -= UINT16_MAX )
                        s += uint64ToString( UINT16_MAX ) + delim;
                    s += uint64ToString( usecs, 10 );
                // Delimiter not needed on the last one
                    if( j<result.rawlen-1 )
                        s += delim;
                }
                r[ "length" ] = c;
                r[ "value" ] = s;
                r.prettyPrintTo( s );
            }
        }

        return s;
    }
    /**
     * @fn String IRRemote::irSendBegin( StringList *args )
     *
     * Set GPIO pin to use when sending an IR command.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String IRRemote::irSendBegin( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
                if( irSend )
                    delete irSend;
                irSend = new IRsend( _atoui(0) );
                irSend->begin();
                break;
            default:
                s = usage( "irSendBegin pin" );
                break;
        }

        return s;
    }
    /**
     * @fn String IRRemote::irSendRaw( StringList *args )
     *
     * Send a raw IR message.
     *
     * @param[in] args Reference to arguments.
     * @return Send bytes.
     */
    String IRRemote::irSendRaw( StringList *args )
    {
        String s;
        uint16_t *data = 0;
        uint16_t c = count( args );

        if( c<1 || !irSend )
            s = usage( "irSendRow [rawdata]+" );
        else
        {
            data = new uint16_t[ c ];

            for( uint16_t i=0; i<c; i++ )
                *(data+i) = _atoui( i );
        // Send IR data using 38Hz
            irSend->sendRaw( data, c, 38 );

            delete [] data;
            s = prettyPrintTo( "value" , c );
        }

        return s;
    }
}
