/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (aoiduino@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/masaomiura/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#ifdef ARDUINO_spresense_ast
#include "spresense-ast.h"
#include <eMMC.h>
#include <Flash.h>
#include <SDHCI.h>
StorageClass *AstStorage = &Flash;
//eMMCClass eMMC;
//FlashClass Flash;
SDClass AstSD;

/** eMMC root path */
#define _EMMC_ "/mnt/emmc"
/** Flash root path */
#define _FLASH_ "/mnt/spif"
/** SD root path */
#define _SD_ "/mnt/sd0"

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
            { "analogRead", &Ast::analogRead },
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
            { "cd", &Ast::cd },
            { "format", &Ast::format },
            { "ll", &Ast::ll },
            { "mkdir", &Ast::mkdir },
            { "pwd", &Ast::pwd },
            { "rmdir", &Ast::rmdir },
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
     * @fn String Ast::analogRead( StringList *args )
     *
     * Reads the value from the specified analog pin.
     *
     * @param[in] args Reference to arguments.
     * @return int (A0 to A5).
     */
    String Ast::analogRead( StringList *args )
    {
        String s;
        int pin = 0;

        switch( count(args) )
        {
            case 1:
                switch( _atoi(0) )
                {
                    case 0:
                        pin = A0;
                        break;
                    case 1:
                        pin = A1;
                        break;
                    case 2:
                        pin = A2;
                        break;
                    case 3:
                        pin = A3;
                        break;
                    case 4:
                        pin = A4;
                        break;
                    case 5:
                        pin = A5;
                        break;
                    default:
                        return s = analogRead( 0 );
                }
                s = prettyPrintTo( "value", ::analogRead(pin) );
                break;
            default:
                s = usage( "analogRead [0-5]" );
                break;
        }

        return s;
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
     * @fn String Ast::cd( StringList *args )
     *
     * Change device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::cd( StringList *args )
    {
        String s;
        String path;

        switch( count(args) )
        {
            case 1:
                path = _a( 0 );
                if( path==_EMMC_ )
                    AstStorage = &eMMC;
                else if( (path==_FLASH_) || (path=="/") )
                    AstStorage = &Flash;
                else if( path==_SD_ )
                    AstStorage = &AstSD;
                else
                    s = cd( 0 );
                break;
            default:
                s = usage( "cd ("+String(_EMMC_)
                             +"|"+String(_FLASH_)
                             +"|"+String(_SD_)+")" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::ll( StringList *args )
     *
     * Return file detail list in current device.
     *
     * @param[in] args Reference to arguments.
     * @return Current device.
     */
    String Ast::ll( StringList *args )
    {
        String s, p;
        DynamicJsonBuffer json;
        JsonArray &r = json.createArray();
        File f;
        StringList *sl, *sm;

        switch( count(args) )
        {
            case 0:
            case 1:
            // Mount point
                f = AstStorage->open( "" );
                p = f.name();
            // Root path
                if( !count(args) )
                    f = AstStorage->open( _a(0) );
            // File info to string
                f = f.openNextFile();
                while( f )
                {
                    if( s.length() )
                        s += ",";
                    s += (f.isDirectory()?"d":"-")
                       + String(":") + String(f.name()).substring(p.length())
                       + String(":") + f.size();
                    f = f.openNextFile();
                }
                f.close();
            // String to JSON
                sl = split( s, "," );
                for( int i=0; i<count(sl); i++ )
                {
                    sm = split( (sl+i)->value, ":" );
                    JsonObject &o = r.createNestedObject();
                    o[ "type" ] = (sm+0)->value;
                    o[ "name" ] = (sm+1)->value;
                    o[ "size" ] = (sm+2)->value;
                    delete [] sm;
                }
                delete [] sl;
                s = "";
                r.prettyPrintTo( s );
                break;
            default:
                s = usage( "ls" );
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
    /**
     * @fn String Ast::mkdir( StringList *args )
     *
     * Make directory on current device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::mkdir( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
                AstStorage->mkdir( _a(0) );
                break;
            default:
                s = usage( "mkdir path" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::pwd( StringList *args )
     *
     * Return current device.
     *
     * @param[in] args Reference to arguments.
     * @return Current device.
     */
    String Ast::pwd( StringList *args )
    {
        String s;
        File f;

        switch( count(args) )
        {
            case 0:
                f = AstStorage->open( "" );
                s = prettyPrintTo( "value" , f.name() );
                f.close();
                break;
            default:
                s = usage( "pwd" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::rmdir( StringList *args )
     *
     * Remove directory on current device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::rmdir( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
                AstStorage->rmdir( _a(0) );
                break;
            default:
                s = usage( "rmdir path" );
                break;
        }

        return s;
    }
}
#endif
