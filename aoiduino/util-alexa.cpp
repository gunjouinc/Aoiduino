/******************************************************************************
**
** Copyright 2009-2023 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#include "base-arduinojson.h"
#include "util-alexa.h"

/**
* @namespace AoiUtil
* @brief Aoi utility classes.
 */
namespace AoiUtil
{
// Static variables.
    Espalexa *Alexa::espalexa = 0;
    /**
     * @fn Alexa::Alexa( void )
     *
     * Constructor. Member variables are initialized.
     */
    Alexa::Alexa( void )
    {
        if( !espalexa )
        {
#if defined(ESP8266) || defined(ESP32)
            espalexa = new Espalexa;
#endif
        }
   }
    /**
     * @fn Alexa::~Alexa( void )
     *
     * Destructor. Member variables are deleted.
     */
    Alexa::~Alexa( void )
    {
    }
    /**
     * @fn String Alexa::alexaAddDevice( StringList *args )
     *
     * Add alexa device.
     *
     * @param[in] args Reference to arguments.
     * @return Name on success.
     */
    String Alexa::alexaAddDevice( StringList *args )
    {
        String s;
        EspalexaDevice *d = 0;

        switch( count(args) )
        {
            case 1:
#if defined(ESP8266) || defined(ESP32)
                for( int i=0; i<ESPALEXA_MAXDEVICES; i++ )
                {
                    d = espalexa->getDevice( i );
                    if( !d )
                    {
                        d = new EspalexaDevice( _a(0), (BrightnessCallbackFunction)(0) );
                        espalexa->addDevice( d );
                        s = prettyPrintTo( "value", d->getName() );
                        break;
                    }
                }
#endif
                break;
            default:
                s = usage( "alexaAddDevice name" );
                break;
        }

        return s;
    }
    /**
     * @fn String Alexa::alexaBegin( StringList *args )
     *
     * Begin alexa function.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Alexa::alexaBegin( StringList *args )
    {
        String s;
        EspalexaDevice *d = 0;

        switch( count(args) )
        {
            case 0:
#if defined(ESP8266) || defined(ESP32)
                espalexa->begin();
#endif
                break;
            default:
                s = usage( "alexaBegin" );
                break;
        }

        return s;
    }
    /**
     * @fn String Alexa::alexaGetValue( StringList *args )
     *
     * Get value usging name.
     *
     * @param[in] args Reference to arguments.
     * @return Current value.
     */
    String Alexa::alexaGetValue( StringList *args )
    {
        String s;
        EspalexaDevice *d = 0;

        switch( count(args) )
        {
            case 1:
#if defined(ESP8266) || defined(ESP32)
                for( int i=0; i<ESPALEXA_MAXDEVICES; i++ )
                {
                    d = espalexa->getDevice( i );
                    if( d && (d->getName()==_a(0)) )
                    {
                        s = prettyPrintTo( "value", d->getValue() );
                        break;
                    }
                }
#endif
                break;
            default:
                s = usage( "alexaGetValue name" );
                break;
        }

        return s;
    }
    /**
     * @fn void Alexa::alexaLoop( StringList *args )
     *
     * Loop method to receive status.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Alexa::alexaLoop( StringList *args )
    {
        String s;
 
        switch( count(args) )
        {
            case 0:
#if defined(ESP8266) || defined(ESP32)
                espalexa->loop();
#endif
                break;
            default:
                s = usage( "alexaLoop" );
                break;
        }

        return s;
    }
}
