/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#ifdef ESP32
/** Esp WiFi timeout */
#define ESP_WIFI_TIMEOUT 30000

#include "esp-esp32.h"
/* Flash */
#include <FS.h>
#include <SPIFFS.h>
FS *EspStorage = &SPIFFS;
/* Ticker */
#include <Ticker.h>
Ticker ticker;

/** Flash root path */
#define _FLASH_ "/mnt/spif"

/**
 * @namespace AoiEsp
 * @brief Aoi esp classes.
 */
namespace AoiEsp
{
// Static variables.
    AoiBase::FunctionTable *Esp32::m_functionTable = 0;
    /**
     * @fn Esp32::Esp32( void )
     *
     * Constructor. Member variables are initialized.
     */
    Esp32::Esp32( void )
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
            { "echo", &Arduino::echo },
            { "micros", &Arduino::micros },
            { "millis", &Arduino::millis },
            { "noTone", &Arduino::noTone },
            { "pinMode", &Arduino::pinMode },
            { "tone", &Arduino::tone },
            { "yield", &Arduino::yield },
            /* File */
            { ">", &Esp32::create },
            { ">>", &Esp32::append },
            { "cat", &Esp32::read },
            { "format", &Esp32::format },
            { "ll", &Esp32::ll },
            { "mkdir", &Esp32::mkdir },
            { "pwd", &Esp32::pwd },
            { "rm", &Esp32::remove },
            { "rmdir", &Esp32::rmdir },
            { "touch", &Esp32::touch },
            /* LowPower */
            { "dmesg", &Esp32::dmesg },
            { "reboot", &Esp32::restart },
            { "sleep", &Esp32::sleep },
        // $ Please set your function to use.
            { "", 0 }
        };
    // Creates function table to avoid kernel panic.
        uint8_t c = sizeof(ftl) / sizeof(AoiBase::FunctionTable);
        m_functionTable = Arduino::functionTable( ftl, c );
    // Initalize library
        SPIFFS.begin();
    }
    /**
     * @fn Esp32::~Esp32( void )
     *
     * Destructor. Member variables are deleted.
     */
    Esp32::~Esp32( void )
    {
    }
    /**
     * @fn String Esp32::className( void )
     *
     * @see bool AbstractBase::className( void )
     */
    String Esp32::className( void )
    {
        return String( "Esp32" );
    }
    /**
     * @fn bool Esp32::isExist( const String &function )
     *
     * @see bool AbstractBase::isExist( const String &function )
     */
    bool Esp32::isExist( const String &function )
    {
        return Arduino::isExist( function, m_functionTable );
    }
    /**
     * @fn String Esp32::practice( StringList *args )
     *
     * @see String AbstractBase::practice( StringList *args )
     */
    String Esp32::practice( StringList *args )
    {
        return Arduino::practice( args, m_functionTable );
    }
    /**
     * @fn String Esp32::usages( void )
     *
     * @see String AbstractBase::usages( void )
     */
    String Esp32::usages( void )
    {
        return Arduino::usages( m_functionTable );
    }
    /**
     * @fn StringList* Esp32::rcScript( const String &index )
     *
     * @see StringList* AbstractBase::rcScript( const String &index )
     */
    StringList* Esp32::rcScript( const String &index )
    {
        StringList *sl = 0;
        String s = appendRootPath( index );

        if( !EspStorage->exists(s) )
            return sl;

        File f = EspStorage->open( s, FILE_READ );
        if( f )
        {
            String s = f.readString();
            f.close();
            sl = split( s, String(_lf) );
        }

        return sl;
    }
    /**
     * @fn String Esp32::append( StringList *args )
     *
     * Append value on current device.
     *
     * @param[in] args Reference to arguments.
     * @return value string.
     */
    String Esp32::append( StringList *args )
    {
        String s, t;
        int c = count( args );

        if( c<2 )
            s = usage( ">> file .+" );
        else
        {
            t = appendRootPath( _a(0) );
        // Create file if need
            if( !EspStorage->exists(t) )
            {
                File w = EspStorage->open( t, FILE_WRITE );
                w.close();
            }
            File f = EspStorage->open( t, FILE_APPEND );
            if( 0<f.size() )
                f.print( _lf );
            for( int i=1; i<c; i++ )
            {
                if( 1<i )
                    f.print( STR_SPACE );
                f.print( _a(i) );
            }
            f.close();
        // Re-open, Can't practice f.seek( 0 )
            f = EspStorage->open( t, FILE_READ );
            s = prettyPrintTo( "value", f.readString() );
            f.close();
         }

         return s;
    }
    /**
     * @fn String Esp32::cd( StringList *args )
     *
     * Change device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp32::cd( StringList *args )
    {
        String s;
        String path;

        switch( count(args) )
        {
            case 1:
                path = _a( 0 );
                if( (path==_FLASH_) || (path=="/") )
                    EspStorage = &SPIFFS;
                else
                    s = cd( 0 );
                break;
            default:
                s = usage( "cd ("+String(_FLASH_)+")" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp32::create( StringList *args )
     *
     * Create value on current device.
     *
     * @param[in] args Reference to arguments.
     * @return value string.
     */
    String Esp32::create( StringList *args )
    {
        String s, t;
        int c = count( args );

        if( c<2 )
            s = usage( "> file .+" );
        else
        {
            t = appendRootPath( _a(0) );
            if( EspStorage->exists(t) )
                EspStorage->remove( t );
            s = append( args );
        }

        return s;
    }
    /**
     * @fn String Esp32::format( StringList *args )
     *
     * Format file device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp32::format( StringList *args )
    {
        String s;
        String path;

        switch( count(args) )
        {
            case 1:
                path = _a( 0 );
                if( path==_FLASH_ )
                    SPIFFS.format();
                else
                    s = format( 0 );
                break;
            default:
                s = usage( "format ("+String(_FLASH_)+")" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp32::ll( StringList *args )
     *
     * Return file detail list in current device.
     *
     * @param[in] args Reference to arguments.
     * @return Current device.
     */
    String Esp32::ll( StringList *args )
    {
        String s, p, root;
        DynamicJsonBuffer json;
        JsonArray &r = json.createArray();
        File d, f;

        switch( count(args) )
        {
            case 1:
                root = _a( 0 );
            case 0:
            // Root path
                root = appendRootPath( root );
                d = EspStorage->open( root );
                p = d.name();
            // File info to JSON
                f = d.openNextFile();
                while( f )
                {
                    JsonObject &o = r.createNestedObject();
                    o[ "type" ] = f.isDirectory() ? "d" : "-";
                    o[ "name" ] = String(f.name()).substring( p.length() );
                    o[ "size" ] = f.size();
                    f.close();
                    f = d.openNextFile();
                }
                s = "";
                r.prettyPrintTo( s );
                break;
            default:
                s = usage( "ll (path)?" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp32::mkdir( StringList *args )
     *
     * Make directory on current device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp32::mkdir( StringList *args )
    {
        String s, t;

        switch( count(args) )
        {
            case 1:
                t = appendRootPath( _a(0) );
                if( EspStorage->exists(t) )
                    s = mkdir( 0 );
                else
                    EspStorage->mkdir( t );
                break;
            default:
                s = usage( "mkdir path" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp32::pwd( StringList *args )
     *
     * Return current device.
     *
     * @param[in] args Reference to arguments.
     * @return Current device.
     */
    String Esp32::pwd( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                s = prettyPrintTo( "value" , _FLASH_ );
                break;
            default:
                s = usage( "pwd" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp32::read( StringList *args )
     *
     * Return file content on current device.
     *
     * @param[in] args Reference to arguments.
     * @return File content.
     */
    String Esp32::read( StringList *args )
    {
        String s, t;
        File f;

        switch( count(args) )
        {
            case 1:
                t = appendRootPath( _a(0) );
                if( !EspStorage->exists(t) )
                    s = read( 0 );
                else
                {
                    f = EspStorage->open( t, FILE_READ );
                    if( f )
                    {
                        s = prettyPrintTo( "value" , f.readString() );
                        f.close();
                    }
                }
                break;
            default:
                s = usage( "cat file" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp32::remove( StringList *args )
     *
     * Remove file on current device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp32::remove( StringList *args )
    {
        String s, t;

        switch( count(args) )
        {
            case 1:
                t = appendRootPath( _a(0) );
                if( !EspStorage->exists(t) )
                    s = remove( 0 );
                else
                    EspStorage->remove( t );
                break;
            default:
                s = usage( "rm file" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp32::rmdir( StringList *args )
     *
     * Remove directory on current device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp32::rmdir( StringList *args )
    {
        String s, t;

        switch( count(args) )
        {
            case 1:
                t = appendRootPath( _a(0) );
                if( !EspStorage->exists(t) )
                    s = rmdir( 0 );
                else
                    EspStorage->rmdir( t );
                break;
            default:
                s = usage( "rmdir path" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp32::touch( StringList *args )
     *
     * Create empty file on current device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp32::touch( StringList *args )
    {
        String s, t;
        File f;

        switch( count(args) )
        {
            case 1:
                t = appendRootPath( _a(0) );
                f = EspStorage->open( t, FILE_WRITE );
                if( f )
                    f.close();
                break;
            default:
                s = usage( "touch file" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp32::dmesg( StringList *args )
     *
     * Returns system information.
     *
     * @param[in] args Reference to arguments.
     * @return System information.
     */
    String Esp32::dmesg( StringList *args )
    {
        String s;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        switch( count(args) )
        {
            case 0:
                r[ "chipId" ] = String( (uint16_t)(ESP.getEfuseMac()>>32), HEX )
                              + String( (uint32_t)ESP.getEfuseMac(), HEX );
                r[ "cpuFreqMHz" ] = ESP.getCpuFreqMHz();
                r[ "flashChipSize" ] = ESP.getFlashChipSize();
                r[ "freeHeap" ] = ESP.getFreeHeap();
                r[ "resetReason0" ] = resetReason( rtc_get_reset_reason(0) );
                r[ "resetReason1" ] = resetReason( rtc_get_reset_reason(1) );
                r[ "sdkVersion" ] = ESP.getSdkVersion();
                r.prettyPrintTo( s );
                break;
            default:
                s = usage( "dmesg" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp32::restart( StringList *args )
     *
     * Reboot the system.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp32::restart( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
                ticker.once_ms( _atoi(0), reboot );
                break;
            default:
                s = usage( "reboot delay(ms)" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp32::sleep( StringList *args )
     *
     * Sleep (yield) this thread.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp32::sleep( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
            // Not work?
                esp_sleep_enable_timer_wakeup( _atoi(0) * 1000 * 1000 );
                break;
            default:
                s = usage( "sleep [0-9]+" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp32::appendRootPath( const String &path )
     *
     * Append root path ("/") if need.
     *
     * @return Path include root.
     */
    String Esp32::appendRootPath( const String &path )
    {
        String s = path;
        String r = "/";

        if( s.indexOf(r) )
            s = r + s;

        return s;
    }
    /**
     * @fn void Esp32::reboot( void )
     *
     * Reboot the system.
     */
    void Esp32::reboot( void )
    {
        ESP.restart();
    }
    /**
     * @fn String Esp32::resetReason( RESET_REASON reason )
     *
     * Returns reset reason string.
     *
     * @return Reset reason string.
     */
    String Esp32::resetReason( RESET_REASON reason )
    {
        String s;

        switch( reason )
        {
            case 1 :
                s = "Vbat power on reset";
                break;
            case 3 :
                s = "Software reset digital core";
                break;
            case 4 :
                s = "Legacy watch dog reset digital core";
                break;
            case 5 :
                s = "Deep Sleep reset digital core";
                break;
            case 6 :
                s = "Reset by SLC module, reset digital core";
                break;
            case 7 :
                s = "Timer Group0 Watch dog reset digital core";
                break;
            case 8 :
                s = "Timer Group1 Watch dog reset digital core";
                break;
            case 9 :
                s = "RTC Watch dog Reset digital core";
                break;
            case 10 :
                s = "Instrusion tested to reset CPU";
                break;
            case 11 :
                s = "Time Group reset CPU";
                break;
            case 12 :
                s = "Software reset CPU";
                break;
            case 13 :
                s = "RTC Watch dog Reset CPU";
                break;
            case 14 :
                s = "for APP CPU, reseted by PRO CPU";
                break;
            case 15 :
                s = "Reset when the vdd voltage is not stable";
                break;
            case 16 :
                s = "RTC Watch dog reset digital core and rtc module";
                break;
            default :
                s = "No mean";
                break;
        }

        return s;
    }
}
#endif
