/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (aoiduino@gunjou.co.jp)
**
******************************************************************************/
#ifdef ARDUINO_spresense_ast
#include "spresense-ast.h"
#include <eMMC.h>
#include <Flash.h>
#include <SDHCI.h>
File AstFile;
//eMMCClass eMMC;
//FlashClass Flash;
SDClass AstSD;

/** eMMC root path */
#define _EMMC_ "/eMMC"
/** Flash root path */
#define _FLASH_ "/Flash"
/** SD root path */
#define _SD_ "/SDHCI"

/**
* @namespace AoiSpresense
* @brief Aoi Spresense classes.
 */
namespace AoiSpresense
{
// Static variables.
    AoiBase::FunctionTable *Ast::m_functionTable = 0;
    /**
     * @fn Ast::Ast( void )
     *
     * Constructor. Member variables are initialized.
     */
    Ast::Ast( void )
    {
        if( m_functionTable )
            return;
    // Sets function table, If there is no instance.
        AoiBase::FunctionTable ftl[] =
        {
        // ^ Please set your function to use.
            /* Arduino Core */
            { "analogRead", &Arduino::analogRead },
            { "analogWrite", &Arduino::analogWrite },
            { "delay", &Arduino::delay },
            { "delayMicroseconds", &Arduino::delayMicroseconds },
            { "digitalRead", &Arduino::digitalRead },
            { "digitalWrite", &Arduino::digitalWrite },
            { "led", &Ast::led },
            { "micros", &Arduino::micros },
            { "millis", &Arduino::millis },
            { "noTone", &Arduino::noTone },
            { "pinMode", &Arduino::pinMode },
            { "tone", &Arduino::tone },
            /* File */
            { "format", &Ast::format },
        // $ Please set your function to use.
            { "", 0 }
        };
    // Creates function table to avoid kernel panic.
        uint8_t c = sizeof(ftl) / sizeof(AoiBase::FunctionTable);
        m_functionTable = Arduino::functionTable( ftl, c );
    }
    /**
     * @fn Ast::~Ast( void )
     *
     * Destructor. Member variables are deleted.
     */
    Ast::~Ast( void )
    {
    }
    /**
     * @fn String Ast::className( void )
     *
     * @see bool Ast::className( void )
     */
    String Ast::className( void )
    {
        return String( "Ast" );
    }
    /**
     * @fn bool Ast::isExist( const String &function )
     *
     * @see bool AbstractBase::isExist( const String &function )
     */
    bool Ast::isExist( const String &function )
    {
        return Arduino::isExist( function, m_functionTable );
    }
    /**
     * @fn String Ast::practice( StringList *args )
     *
     * @see String AbstractBase::practice( StringList *args )
     */
    String Ast::practice( StringList *args )
    {
        return Arduino::practice( args, m_functionTable );
    }
    /**
     * @fn String Ast::usages( void )
     *
     * @see String AbstractBase::usages( void )
     */
    String Ast::usages( void )
    {
        return Arduino::usages( m_functionTable );
    }
    /**
     * @fn String Ast::led( StringList *args )
     *
     * Turn led on or off.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::led( StringList *args )
    {
        String s;
        uint8_t i = 0;

        switch( count(args) )
        {
            case 2:
                i = _atoi( 0 );
                switch( i )
                {
                    case 1:
                        i = LED1;
                        break;
                    case 2:
                        i = LED2;
                        break;
                    case 3:
                        i = LED3;
                        break;
                    default:
                        i = LED0;
                        break;
                }
            // Sets pin mode output to enable led.
                ::pinMode( i, OUTPUT );
                if( _a(1)=="ON" )
                    ::digitalWrite( i, HIGH );
                else
                    ::digitalWrite( i, LOW );
                break;
            default:
                s = usage( "led 0-3 (ON|OFF)" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::format( StringList *args )
     *
     * Format file device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::format( StringList *args )
    {
        String s;
        String path;

        switch( count(args) )
        {
            case 1:
                path = _a( 0 );
                if( path==_EMMC_ )
                    eMMC.format();
                else if( path==_FLASH_ )
                    Flash.format();
                else if( path==_SD_ )
                    AstSD.format();
                else
                    s = format( 0 );
                break;
            default:
                s = usage( "format ("+String(_EMMC_)
                                 +"|"+String(_FLASH_)
                                 +"|"+String(_SD_)+")" );
                break;
        }

        return s;
    }
}
#endif
