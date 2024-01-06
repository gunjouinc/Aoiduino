/******************************************************************************
**
** Copyright 2009-2024 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#include "util-mqtt.h"

/**
* @namespace AoiUtil
* @brief Aoi utility classes.
 */
namespace AoiUtil
{
// Static variables.
    MqttClient *Mqtt::mqtt = 0;
    /**
     * @fn Mqtt::Mqtt( void )
     *
     * Constructor. Member variables are initialized.
     */
    Mqtt::Mqtt( void )
    {
    }
    /**
     * @fn MqttClient::~Mqtt( void )
     *
     * Destructor. Member variables are deleted.
     */
    Mqtt::~Mqtt( void )
    {
    }
    /**
     * @fn String Mqtt::mqttConnect( StringList *args )
     *
     * Connect to mqtt broker.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Mqtt::mqttConnect( StringList *args )
    {
        String s;
        int port = 1883;

        switch( count(args) )
        {
            case 2:
                port = _atoi( 1 );
            case 1:
                mqtt->setId( STR_AOIDUINO );
                if( !mqtt->connect(_a(0).c_str(),port) )
                    return mqttConnect( 0 );
                break;
            default:
                s = usage( "mqttConnect broker (port)?" );
                break;
        }

        return s;
    }
    /**
     * @fn String Mqtt::mqttPoll( StringList *args )
     *
     * Poll subscribed message from topic.
     *
     * @param[in] args Reference to arguments.
     * @return Subscribed message.
     */
    String Mqtt::mqttPoll( StringList *args )
    {
        String s;
        int start = 0;
        int timeout = 180;

        switch( count(args) )
        {
            case 1:
                timeout = _atoi( 0 );
            case 0:
                start = ::millis() / 1000;
                while( true )
                {
                    int size = mqtt->parseMessage();
                    if( size )
                    {
                        while( mqtt->available() )
                            s += (char)mqtt->read();
                        break;
                    }
                // timeout
                    else if( timeout<((::millis()/1000)-start) )
                        break;
                }
                s = prettyPrintTo( "value" , s );
                break;
            default:
                s = usage( "mqttPoll (timeout)?" );
                break;
        }

        return s;
    }
    /**
     * @fn String Mqtt::mqttPublish( StringList *args )
     *
     * Publish message to topic.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Mqtt::mqttPublish( StringList *args )
    {
        String s;
        int c = count( args );
        int timeout = 180;

        if( c<3 )
            s = usage( "mqttPublish topic [0-2] message" );
        else
        {
            int start = ::millis() / 1000;
            int i = 0;
        // restore argument after qos
            String t = join( args, STR_SPACE, 2 );
            char *buf = (char*)t.c_str();
        // TX_PAYLOAD_BUFFER_SIZE is defined in MqttClient.cpp
            while( i<t.length() )
            {
                mqtt->beginMessage( _a(0), false, _atoi(1) );
                i += mqtt->print( buf+i );
                mqtt->endMessage();
            // timeout
                if( timeout<((::millis()/1000)-start) )
                    break;
            }
        }

        return s;
    }
    /**
     * @fn String Mqtt::mqttSubscribe( StringList *args )
     *
     * Start subscribe message from topic.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Mqtt::mqttSubscribe( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 2:
                if( !mqtt->subscribe(_a(0),_atoui(1)) )
                    return mqttSubscribe( 0 );
                break;
            default:
                s = usage( "mqttSubscribe topic [0-2]" );
                break;
        }

        return s;
    }
}
