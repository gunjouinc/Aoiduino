/******************************************************************************
**
** Copyright 2009-2022 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#ifdef ARDUINO_M5STACK_Core2
#include "esp-m5core2.h"
/* M5Core2 */
#include <M5Core2.h>
// Height of text to be printed and scrolled
#define _LINE_HEIGHT_ 8
// Number of lines in top fixed area (lines counted from top of screen)
#define _TOP_FIXED_AREA_ 0
// Number of lines in bottom fixed area (lines counted from bottom of screen)
#define _BOTOM_FIXED_AREA_ 0
// Bottom of screen area
#define _BOTTOM_MAX_ 240

/**
 * @namespace AoiEsp
 * @brief Aoi esp classes.
 */
namespace AoiEsp
{
// Static variables.
    AoiBase::FunctionTable *M5Core2::m_functionTable = 0;
    uint16_t M5Core2::m_leftCoordinate = 0;
    uint16_t M5Core2::m_topCoordinate = 0;
    uint8_t M5Core2::m_fontSize = 2;
    /**
     * @fn M5Core2::M5Core2( void )
     *
     * Constructor. Member variables are initialized.
     */
    M5Core2::M5Core2( void )
    {
        if( m_functionTable )
            return;
    // Sets function table, If there is no instance.
        AoiBase::FunctionTable ftl[] =
        {
        // ^ Please set your function to use.
        /* LCD */
        { "clear", &M5Core2::clear },
        { "print", &M5Core2::print },
        { "printcr", &M5Core2::printcr },
        { "println", &M5Core2::println },
        { "setBrightness", &M5Core2::setBrightness },
        { "setFontSize", &M5Core2::setFontSize },
        { "sleep", &M5Core2::sleep },
        { "wakeup", &M5Core2::wakeup },
        // $ Please set your function to use.
            { "", 0 }
        };
    // Creates function table.
        uint8_t c = sizeof(ftl) / sizeof(AoiBase::FunctionTable);
        m_functionTable = Arduino::functionTable( ftl, c );
    // Initalize library
        /* M5Stack */
        M5.begin( true, true, true, true );
        M5.Lcd.setRotation( 1 );
        M5.Lcd.fillScreen( TFT_BLACK );
        M5.Lcd.setTextColor( TFT_WHITE, TFT_BLACK );
        setupScrollArea( 0, 0 );
    }
    /**
     * @fn M5Core2::~M5Core2( void )
     *
     * Destructor. Member variables are deleted.
     */
    M5Core2::~M5Core2( void )
    {
    }
    /**
     * @fn String M5Core2::className( void )
     *
     * @see bool AbstractBase::className( void )
     */
    String M5Core2::className( void )
    {
        return String( "M5Core2" );
    }
    /**
     * @fn bool M5Core2::isExist( const String &function )
     *
     * @see bool AbstractBase::isExist( const String &function )
     */
    bool M5Core2::isExist( const String &function )
    {
        return Arduino::isExist( function, m_functionTable );
    }
    /**
     * @fn String M5Core2::practice( StringList *args )
     *
     * @see String AbstractBase::practice( StringList *args )
     */
    String M5Core2::practice( StringList *args )
    {
        return Arduino::practice( args, m_functionTable );
    }
    /**
     * @fn String M5Core2::usages( void )
     *
     * @see String AbstractBase::usages( void )
     */
    String M5Core2::usages( void )
    {
        return Arduino::usages( m_functionTable );
    }
    /**
     * @fn StringList* M5Core2::rcScript( const String &index )
     *
     * @see StringList* AbstractBase::rcScript( const String &index )
     */
    StringList* M5Core2::rcScript( const String &index )
    {
        StringList *sl = 0;

        // Nothing to do

        return sl;
    }
    /**
     * @fn String M5Core2::clear( StringList *args )
     *
     * Clear LCD and move to first position.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String M5Core2::clear( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                M5.Lcd.fillScreen( TFT_BLACK );
                m_leftCoordinate = 0;
                m_topCoordinate = 0;
                break;
            default:
                s = usage( "clear" );
                break;
        }

        return s;
    }
    /**
     * @fn String M5Core2::print( StringList *args )
     *
     * Print text to LCD.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String M5Core2::print( StringList *args )
    {
        String s, t;
        int c = count( args );

        if( c<1 )
            s = usage( "print .+" );
        else
        {
            t = join( args, STR_SPACE );
            for( int i=0; i<t.length(); i++ )
            {
                char data = *(t.c_str()+i);

                if( data==_lf )
                    scrollLine();
                else
                {
                    m_leftCoordinate +=
                        M5.Lcd.drawChar( data,
                                         m_leftCoordinate, m_topCoordinate,
                                         m_fontSize );
                }
            }
        }

        return s;
    }
    /**
     * @fn String M5Core2::printcr( StringList *args )
     *
     * Print text to LCD and go to first column.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String M5Core2::printcr( StringList *args )
    {
        String s, t;
        int c = count( args );
        uint16_t l = m_leftCoordinate;

        if( c<1 )
            s = usage( "printcr .+" );
        else
        {
            t = join( args, STR_SPACE );
            for( int i=0; i<t.length(); i++ )
            {
                char data = *(t.c_str()+i);

                if( data!=_lf )
                {
                    m_leftCoordinate +=
                        M5.Lcd.drawChar( data,
                                         m_leftCoordinate, m_topCoordinate,
                                         m_fontSize );
                }
            }
            m_leftCoordinate = l;
        }

        return s;
    }
    /**
     * @fn String M5Core2::println( StringList *args )
     *
     * Print text to LCD and go to next line.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String M5Core2::println( StringList *args )
    {
        String s, t;
        int c = count( args );

        if( c<1 )
            s = usage( "println .+" );
        else
        {
            (args+c-1)->value += _lf;
            s = print( args );
        }

        return s;
    }
    /**
     * @fn String M5Core2::setBrightness( StringList *args )
     *
     * Set brightness on LCD.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String M5Core2::setBrightness( StringList *args )
    {
        String s;
        int i = 0;

        switch( count(args) )
        {
            case 1:
                i = _atoi( 0 );
                if( !i )
                    M5.Axp.SetDCDC3( false );
                else if( 2500<=i && i<=3300 )
                {
                    M5.Axp.SetDCDC3( true );
                    M5.Axp.SetLcdVoltage( i );
                }
                else
                    s = setBrightness( 0 );
                break;
            default:
                s = usage( "setBrightness [0,2500-3300]" );
                break;
        }

        return s;
    }
    /**
     * @fn String M5Core2::setFontSize( StringList *args )
     *
     * Set current font size
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String M5Core2::setFontSize( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
                m_fontSize = _atoi( 0 );
                break;
            default:
                s = usage( "setFontSize (1|2|4)" );
                break;
        }

        return s;
    }
    /**
     * @fn String M5Core2::sleep( StringList *args )
     *
     * Set LCD mode to sleep.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String M5Core2::sleep( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                M5.Lcd.sleep();
                break;
            default:
                s = usage( "sleep" );
                break;
        }

        return s;
    }
    /**
     * @fn String M5Core2::wakeup( StringList *args )
     *
     * Set LCD mode to wakeup.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String M5Core2::wakeup( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                M5.Lcd.wakeup();
                break;
            default:
                s = usage( "wakeup" );
                break;
        }

        return s;
    }
    /**
     * @fn void M5Core2::scrollLine( void )
     *
     * Scrolls line buffer.
     */
    void M5Core2::scrollLine( void )
    {
        uint16_t h = lineHeight();

        m_leftCoordinate = 0;
        m_topCoordinate += h;
   // The value must wrap around as the screen memory is a circular buffer.
        if( _BOTTOM_MAX_<=m_topCoordinate )
            m_topCoordinate = 0;
   // Scroll the display, Vertical scrolling pointer
        uint16_t address = m_topCoordinate + h;
        M5.Lcd.writecommand( ILI9341_VSCRSADD );
        M5.Lcd.writedata( address>>8 );
        M5.Lcd.writedata( address );
   // Erase buffer.
        M5.Lcd.fillRect( 0, m_topCoordinate, 320, h, TFT_BLACK );
    }
    /**
     * @fn void M5Core2::setupScrollArea( uint16_t top, uint16_t bottom )
     *
     * Setup a portion of the screen for vertical scrolling.
     */
    void M5Core2::setupScrollArea( uint16_t top, uint16_t bottom )
    {
    // Vertical scroll definition.
        M5.Lcd.writecommand( ILI9341_VSCRDEF );
    // Top Fixed Area line count.
        M5.Lcd.writedata( top >> 8 );
        M5.Lcd.writedata( top );
    // Vertical Scrolling Area line count.
        M5.Lcd.writedata( (_BOTTOM_MAX_-top-bottom)>>8 );
        M5.Lcd.writedata( _BOTTOM_MAX_-top-bottom );
    // Bottom Fixed Area line count.
        M5.Lcd.writedata( bottom >> 8 );
        M5.Lcd.writedata( bottom );
    }
    /**
     * @fn uint16_t M5Core2::lineHeight( void )
     *
     * Return line heigt using current font size.
     *
     * @return Line height.
     */
    uint16_t M5Core2::lineHeight( void )
    {
        return _LINE_HEIGHT_ * m_fontSize;
    }
}
#endif
