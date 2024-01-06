/******************************************************************************
**
** Copyright 2009-2024 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#ifdef ARDUINO_M5Stick_C
#include "esp-m5stick-c.h"
/* M5Stick-C */
#include <M5StickC.h>
// Height of text to be printed and scrolled
#define _LINE_HEIGHT_ 8
// Number of lines in top fixed area (lines counted from top of screen)
#define _TOP_FIXED_AREA_ 0
// Number of lines in bottom fixed area (lines counted from bottom of screen)
#define _BOTOM_FIXED_AREA_ 0
// Bottom of screen area
#define _BOTTOM_MAX_ 80

/**
 * @namespace AoiEsp
 * @brief Aoi esp classes.
 */
namespace AoiEsp
{
// Static variables.
    AoiBase::FunctionTable *M5Stick_C::m_functionTable = 0;
    uint16_t M5Stick_C::m_leftCoordinate = 0;
    uint16_t M5Stick_C::m_topCoordinate = 0;
    uint8_t M5Stick_C::m_fontSize = 2;
    /**
     * @fn M5Stick_C::M5Stick_C( void )
     *
     * Constructor. Member variables are initialized.
     */
    M5Stick_C::M5Stick_C( void )
    {
        if( m_functionTable )
            return;
    // Sets function table, If there is no instance.
        AoiBase::FunctionTable ftl[] =
        {
        // ^ Please set your function to use.
        /* LCD */
        { "clear", &M5Stick_C::clear },
        { "print", &M5Stick_C::print },
        { "printcr", &M5Stick_C::printcr },
        { "println", &M5Stick_C::println },
        { "setFontSize", &M5Stick_C::setFontSize },
        { "sleep", &M5Stick_C::sleep },
        { "wakeup", &M5Stick_C::wakeup },
        /* IR */
        { "irReceiveBegin", &AoiUtil::IRRemote::irReceiveBegin },
        { "irReceiveRaw", &AoiUtil::IRRemote::irReceiveRaw },
        { "irSendBegin", &AoiUtil::IRRemote::irSendBegin },
        { "irSendRaw", &AoiUtil::IRRemote::irSendRaw },
        // $ Please set your function to use.
        { "", 0 }
        };
    // Creates function table.
        uint8_t c = sizeof(ftl) / sizeof(AoiBase::FunctionTable);
        m_functionTable = Arduino::functionTable( ftl, c );
    // Initalize library
        /* M5Stack */
        M5.begin( true, true, true );
        M5.Lcd.setRotation( 1 );
        M5.Lcd.fillScreen( TFT_BLACK );
        M5.Lcd.setTextColor( TFT_WHITE, TFT_BLACK );
        setupScrollArea( 0, 0 );
    }
    /**
     * @fn M5Stick_C::~M5Stick_C( void )
     *
     * Destructor. Member variables are deleted.
     */
    M5Stick_C::~M5Stick_C( void )
    {
    }
    /**
     * @fn String M5Stick_C::className( void )
     *
     * @see bool AbstractBase::className( void )
     */
    String M5Stick_C::className( void )
    {
        return String( "M5Stick-C" );
    }
    /**
     * @fn bool M5Stick_C::isExist( const String &function )
     *
     * @see bool AbstractBase::isExist( const String &function )
     */
    bool M5Stick_C::isExist( const String &function )
    {
        return Arduino::isExist( function, m_functionTable );
    }
    /**
     * @fn String M5Stick_C::practice( StringList *args )
     *
     * @see String AbstractBase::practice( StringList *args )
     */
    String M5Stick_C::practice( StringList *args )
    {
        return Arduino::practice( args, m_functionTable );
    }
    /**
     * @fn String M5Stick_C::usages( void )
     *
     * @see String AbstractBase::usages( void )
     */
    String M5Stick_C::usages( void )
    {
        return Arduino::usages( m_functionTable );
    }
    /**
     * @fn StringList* M5Stick_C::rcScript( const String &index )
     *
     * @see StringList* AbstractBase::rcScript( const String &index )
     */
    StringList* M5Stick_C::rcScript( const String &index )
    {
        StringList *sl = 0;

        // Nothing to do

        return sl;
    }
    /**
     * @fn String M5Stick_C::clear( StringList *args )
     *
     * Clear LCD and move to first position.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String M5Stick_C::clear( StringList *args )
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
     * @fn String M5Stick_C::print( StringList *args )
     *
     * Print text to LCD.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String M5Stick_C::print( StringList *args )
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
     * @fn String M5Stick_C::printcr( StringList *args )
     *
     * Print text to LCD and go to first column.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String M5Stick_C::printcr( StringList *args )
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
     * @fn String M5Stick_C::println( StringList *args )
     *
     * Print text to LCD and go to next line.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String M5Stick_C::println( StringList *args )
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
     * @fn String M5Stick_C::setFontSize( StringList *args )
     *
     * Set current font size
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String M5Stick_C::setFontSize( StringList *args )
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
     * @fn String M5Stick_C::sleep( StringList *args )
     *
     * Set LCD mode to sleep.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String M5Stick_C::sleep( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                M5.Axp.SetLDO2( false );
                M5.Axp.SetLDO3( false );
                break;
            default:
                s = usage( "sleep" );
                break;
        }

        return s;
    }
    /**
     * @fn String M5Stick_C::wakeup( StringList *args )
     *
     * Set LCD mode to wakeup.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String M5Stick_C::wakeup( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                M5.Axp.SetLDO2( true );
                M5.Axp.SetLDO3( true );
                break;
            default:
                s = usage( "wakeup" );
                break;
        }

        return s;
    }
    /**
     * @fn void M5Stick_C::scrollLine( void )
     *
     * Scrolls line buffer.
     */
    void M5Stick_C::scrollLine( void )
    {
        uint16_t h = lineHeight();

        m_leftCoordinate = 0;
        m_topCoordinate += h;
   // The value must wrap around as the screen memory is a circular buffer.
        if( _BOTTOM_MAX_<=m_topCoordinate )
            m_topCoordinate = 0;
   // Scroll the display, Vertical scrolling pointer
        uint16_t address = m_topCoordinate + h;
        M5.Lcd.writecommand( ST7735_VSCRDEF );
        M5.Lcd.writedata( address>>8 );
        M5.Lcd.writedata( address );
   // Erase buffer.
        M5.Lcd.fillRect( 0, m_topCoordinate, 160, h, TFT_BLACK );
    }
    /**
     * @fn void M5Stick_C::setupScrollArea( uint16_t top, uint16_t bottom )
     *
     * Setup a portion of the screen for vertical scrolling.
     */
    void M5Stick_C::setupScrollArea( uint16_t top, uint16_t bottom )
    {
    // Vertical scroll definition.
        M5.Lcd.writecommand( ST7735_VSCRDEF );
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
     * @fn uint16_t M5Stick_C::lineHeight( void )
     *
     * Return line heigt using current font size.
     *
     * @return Line height.
     */
    uint16_t M5Stick_C::lineHeight( void )
    {
        return _LINE_HEIGHT_ * m_fontSize;
    }
}
#endif
