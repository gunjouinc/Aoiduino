/******************************************************************************
**
** Copyright 2009-2023 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#include "util-neopixel.h"

#include <Adafruit_NeoPixel.h>

/**
* @namespace AoiUtil
* @brief Aoi utility classes.
 */
namespace AoiUtil
{
// Static variables.
    Adafruit_NeoPixel *NeoPixel::m_neoPixel = 0;
    /**
     * @fn NeoPixel::NeoPixel( void )
     *
     * Constructor. Member variables are initialized.
     */
    NeoPixel::NeoPixel( void )
    {
    }
    /**
     * @fn NeoPixel::~NeoPixel( void )
     *
     * Destructor. Member variables are deleted.
     */
    NeoPixel::~NeoPixel( void )
    {
    }
    /**
     * @fn String NeoPixel::neoPixelBegin( StringList *args )
     *
     * Initialize the Adafruit_NeoPixel using pin number.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String NeoPixel::neoPixelBegin( StringList *args )
    {
        String s;
        StringList sl;

        switch( count(args) )
        {
            case 2:
                neoPixelEnd( &sl );
                m_neoPixel = new Adafruit_NeoPixel( _atoi(0), _atoi(1), NEO_GRB + NEO_KHZ800 );
                m_neoPixel->begin();
                break;
            default:
                s = usage( "neoPixelBegin num pin" );
                break;
        }

        return s;
    }
    /**
     * @fn String NeoPixel::neoPixelClear( StringList *args )
     *
     * Fill the whole NeoPixel strip with 0 / black / off.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String NeoPixel::neoPixelClear( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                m_neoPixel->clear();
                break;
            default:
                s = usage( "neoPixelClear" );
                break;
        }

        return s;
    }
    /**
     * @fn String NeoPixel::neoPixelEnd( StringList *args )
     *
     * Delete Adafruit_NeoPixel instance.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String NeoPixel::neoPixelEnd( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                if( m_neoPixel )
                {
                    delete m_neoPixel;
                    m_neoPixel = 0;
                }
                break;
            default:
                s = usage( "neoPixelEnd" );
                break;
        }

        return s;
    }
    /**
     * @fn String NeoPixel::neoPixelFill( StringList *args )
     *
     * Set all pixel's color using a 32-bit 'packed' RGB or RGBW value.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String NeoPixel::neoPixelFill( StringList *args )
    {
        String s;
        uint16_t i = 0;

        switch( count(args) )
        {
            case 1:
                for( i=0; i<m_neoPixel->numPixels(); i++ )
                    m_neoPixel->setPixelColor( i, _atolh(0) );
                break;
            default:
                s = usage( "neoPixelFill hexColor" );
                break;
        }

        return s;
    }
    /**
     * @fn String NeoPixel::neoPixelSetBrightness( StringList *args )
     *
     * Set LED brightness.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String NeoPixel::neoPixelSetBrightness( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
                m_neoPixel->setBrightness( _atoi(0) );
                break;
            default:
                s = usage( "neoPixelSetBrightness 0-255" );
                break;
        }

        return s;
    }
    /**
     * @fn String NeoPixel::neoPixelSetPixelColor( StringList *args )
     *
     * Set a pixel's color using a 32-bit 'packed' RGB or RGBW value.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String NeoPixel::neoPixelSetPixelColor( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 2:
                m_neoPixel->setPixelColor( _atoi(0), _atolh(1) );
                break;
            default:
                s = usage( "neoPixelSetPixelColor index hexColor" );
                break;
        }

        return s;
    }
    /**
     * @fn String NeoPixel::neoPixelShow( StringList *args )
     *
     * Transmit pixel data in RAM to NeoPixels.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String NeoPixel::neoPixelShow( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                m_neoPixel->show();
                break;
            default:
                s = usage( "neoPixelShow" );
                break;
        }

        return s;
    }
}
