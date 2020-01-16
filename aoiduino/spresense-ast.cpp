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
/* Flash */
#include <eMMC.h>
#include <Flash.h>
#include <SDHCI.h>
StorageClass *AstStorage = &Flash;
//eMMCClass eMMC;
//FlashClass Flash;
SDClass AstSD;
/* GNSS */
#include <GNSS.h>
SpGnss Gnss;
/* LowPower */
#include <LowPower.h>
/* LTE */
#include <LTE.h>
LTE Lte;
LTEScanner LteScanner;
LTEModemVerification LteModem;

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
            { "echo", &Ast::print },
            { "led", &Ast::led },
            { "micros", &Arduino::micros },
            { "millis", &Arduino::millis },
            { "noTone", &Arduino::noTone },
            { "pinMode", &Arduino::pinMode },
            { "tone", &Arduino::tone },
            /* File */
            { "cat", &Ast::read },
            { "cd", &Ast::cd },
            { "ll", &Ast::ll },
            { "format", &Ast::format },
            { "mkdir", &Ast::mkdir },
            { "pwd", &Ast::pwd },
            { "rm", &Ast::remove },
            { "rmdir", &Ast::rmdir },
            { "touch", &Ast::touch },
            /* GNSS */
            { "gnssBegin", &Ast::gnssBegin },
            { "gnssEnd", &Ast::gnssEnd },
            { "gnssConfig", &Ast::gnssNavData },
            { "gnssSattellites", &Ast::gnssSattellites },
            /* LowPower */
            { "clockMode", &Ast::clockMode },
            { "coldSleep", &Ast::coldSleep },
            { "deepSleep", &Ast::deepSleep },
            { "dmesg", &Ast::dmesg },
            { "reboot", &Ast::reboot },
            { "sleep", &Ast::sleep },
            /* LTE */
            { "lteBegin", &Ast::lteBegin },
            { "lteConfig", &Ast::lteConfig },
            { "lteEnd", &Ast::lteEnd },
        // $ Please set your function to use.
            { "", 0 }
        };
    // Creates function table to avoid kernel panic.
        uint8_t c = sizeof(ftl) / sizeof(AoiBase::FunctionTable);
        m_functionTable = Arduino::functionTable( ftl, c );
    // Initalize library
        /* LowPower */
        LowPower.begin();
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
        File d, f;
        StringList *sl, *sm;

        switch( count(args) )
        {
            case 0:
            case 1:
            // Root path
                d = AstStorage->open( "" );
                p = d.name();
                if( count(args) )
                {
                    if( !AstStorage->exists(_a(0)) )
                        return mkdir( 0 );
                    d = AstStorage->open( _a(0) );
                    p = d.name() + String("/");
                }
            // File info to string
                f = d.openNextFile();
                while( f )
                {
                    if( s.length() )
                        s += ",";
                    s += (f.isDirectory()?"d":"-")
                       + String(":") + String(f.name()).substring(p.length())
                       + String(":") + f.size();
                    f = d.openNextFile();
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
                s = usage( "ll" );
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
                if( AstStorage->exists(_a(0)) )
                    s = mkdir( 0 );
                else
                    AstStorage->mkdir( _a(0) );
                break;
            default:
                s = usage( "mkdir path" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::print( StringList *args )
     *
     * Print value or write value on current device.
     *
     * @param[in] args Reference to arguments.
     * @return value string.
     */
    String Ast::print( StringList *args )
    {
        String s;
        File f;

        switch( count(args) )
        {
            case 1:
                s = Arduino::echo( args );
                break;
            case 3:
                if( (_a(1)!=">") && (_a(1)!=">>") )
                    s = echo( 0 );
                else
                {
                    if( (_a(1)==">") && AstStorage->exists(_a(2)) )
                        AstStorage->remove( _a(2) );
                    f = AstStorage->open( _a(2), FILE_WRITE );
                    if( f )
                    {
                        f.print( _a(0) );
                        f.close();
                        s = prettyPrintTo( "value", _a(0) );
                    }
                }
                break;
            default:
                s = usage( "echo value ((>|>>) file)?" );
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
     * @fn String Ast::read( StringList *args )
     *
     * Return file content on current device.
     *
     * @param[in] args Reference to arguments.
     * @return File content.
     */
    String Ast::read( StringList *args )
    {
        String s;
        File f;

        switch( count(args) )
        {
            case 1:
                if( !AstStorage->exists(_a(0)) )
                    s = read( 0 );
                else
                {
                    f = AstStorage->open( _a(0), FILE_READ );
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
     * @fn String Ast::remove( StringList *args )
     *
     * Remove file on current device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::remove( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
                if( !AstStorage->exists(_a(0)) )
                    s = remove( 0 );
                else
                    AstStorage->remove( _a(0) );
                break;
            default:
                s = usage( "rm file" );
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
                if( !AstStorage->exists(_a(0)) )
                    s = rmdir( 0 );
                else
                    AstStorage->rmdir( _a(0) );
                break;
            default:
                s = usage( "rmdir path" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::touch( StringList *args )
     *
     * Create empty file on current device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::touch( StringList *args )
    {
        String s;
        File f;

        switch( count(args) )
        {
            case 1:
                f = AstStorage->open( _a(0), FILE_WRITE );
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
     * @fn String Ast::gnssBegin( StringList *args )
     *
     * Begin GNSS function.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::gnssBegin( StringList *args )
    {
        String s;
        int r;

        switch( count(args) )
        {
            case 1:
                r = Gnss.begin();
                if( r )
                    return gnssBegin( 0 );
                switch( _atoi(0) )
                {
                    case 0:
                    // GPS, World wide coverage
                        Gnss.select( GPS );
                        break;
                    case 1:
                    // GLONASS, World wide coverage
                        Gnss.select( GLONASS );
                        break;
                    case 2:
                    // GPS+SBAS, North America
                        Gnss.select( GPS );
                        Gnss.select( SBAS );
                        break;
                    case 3:
                    // GPS+Glonass
                        Gnss.select( GPS );
                        Gnss.select( GLONASS );
                        break;
                    case 4:
                    // GPS+QZSS_L1CA, East Asia & Oceania
                        Gnss.select( GPS );
                        Gnss.select( QZ_L1CA );
                        break;
                    case 5:
                    // GPS+Glonass+QZSS_L1CA, East Asia & Oceania
                        Gnss.select( GPS );
                        Gnss.select( GLONASS );
                        Gnss.select( QZ_L1CA );
                        break;
                    case 6:
                    // GPS+QZSS_L1CA+QZSS_L1S, Japan
                        Gnss.select( GPS );
                        Gnss.select( QZ_L1CA );
                        Gnss.select( QZ_L1S );
                        break;
                    default:
                        return gnssBegin( 0 );
                }
                r = Gnss.start( COLD_START );
                if( r )
                    s = gnssBegin( 0 );
                break;
            default:
                s = usage( "gnssBegin [0-6]" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::gnssEnd( StringList *args )
     *
     * End GNSS function.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::gnssEnd( StringList *args )
    {
        String s;
        int r;

        switch( count(args) )
        {
            case 0:
                r = Gnss.stop();
                if( r )
                    s = gnssEnd( 0 );
                else
                {
                    r = Gnss.end();
                    if( r )
                        s = gnssEnd( 0 );
                }
                break;
            default:
                s = usage( "gnssEnd" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::gnssNavData( StringList *args )
     *
     * Print GNSS navidation data.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::gnssNavData( StringList *args )
    {
        String s;
        char buf[ 128 ];
        SpNavData nav;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        switch( count(args) )
        {
            case 0:
                if( !Gnss.waitUpdate(-1) )
                    s = gnssNavData( 0 );
                else
                {
                    Gnss.getNavData( &nav );
                    if( !nav.posDataExist )
                        s = gnssNavData( 0 );
                    else
                    {
                        r[ "numSatellites" ] = (int)nav.numSatellites;
                        r[ "fixMode" ] = (nav.posFixMode==FixInvalid) ? "No-Fix" : "Fix";
                        snprintf( buf, sizeof(buf),
                                  "%04d-%02d-%02d %02d:%02d:%02d.%06d",
                                  nav.time.year, nav.time.month, nav.time.day,
                                  nav.time.hour, nav.time.minute, nav.time.sec, nav.time.usec );
                        r[ "dateTime" ] = buf;
                        r[ "direction" ] = nav.direction;
                        r[ "latitude" ] = nav.latitude;
                        r[ "longitude" ] = nav.longitude;
                        r[ "velocity" ] = nav.velocity;
                        r.prettyPrintTo( s );
                    }
                }
                break;
            default:
                s = usage( "gnssConfig" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::gnssSattellites( StringList *args )
     *
     * Print GNSS sattellites.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::gnssSattellites( StringList *args )
    {
        String s, type = "---";
        SpNavData nav;
        DynamicJsonBuffer json;
        JsonArray &r = json.createArray();

        switch( count(args) )
        {
            case 0:
                if( !Gnss.waitUpdate(-1) )
                    s = gnssSattellites( 0 );
                else
                {
                    Gnss.getNavData( &nav );
                    if( !nav.posDataExist )
                        s = gnssSattellites( 0 );
                    else
                    {
                        for( int i=0; i<nav.numSatellites; i++ )
                        {
                            JsonObject &o = r.createNestedObject();

                            switch( nav.getSatelliteType(i) )
                            {
                                case GPS:
                                    type = "GPS";
                                    break;
                                case GLONASS:
                                    type = "GLN";
                                    break;
                                case QZ_L1CA:
                                    type = "QCA";
                                    break;
                                case SBAS:
                                    type = "SBA";
                                    break;
                                case QZ_L1S:
                                    type = "Q1S";
                                    break;
                                default:
                                    type = "UKN";
                                    break;
                            }
                            o[ "type" ] = type;
                            o[ "id" ] = (int)nav.getSatelliteId( i );
                            o[ "azimuth" ] = (int)nav.getSatelliteAzimuth( i );
                            o[ "elevation" ] = (int)nav.getSatelliteElevation( i );
                            o[ "signalLevel" ] = nav.getSatelliteSignalLevel( i );
                        }
                    }
                    r.prettyPrintTo( s );
                }
                break;
            default:
                s = usage( "gnssSattellites" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::clockMode( StringList *args )
     *
     * Set clock mode and change system clock dynamically.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::clockMode( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
                LowPower.clockMode( static_cast<clockmode_e>(_atoi(0)) );
                break;
            default:
                s = usage( "clockMode [0-2]" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::coldSleep( StringList *args )
     *
     * Enter the cold sleep state.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::coldSleep( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                LowPower.coldSleep();
                break;
            case 1:
                LowPower.coldSleep( _atoui(0) );
                break;
            default:
                s = usage( "coldSleep [0-9]*" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::deepSleep( StringList *args )
     *
     * Enter the deep sleep state.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::deepSleep( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                LowPower.deepSleep();
                break;
            case 1:
                LowPower.deepSleep( _atoui(0) );
                break;
            default:
                s = usage( "deepSleep [0-9]*" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::dmesg( StringList *args )
     *
     * Returns system information.
     *
     * @param[in] args Reference to arguments.
     * @return System information.
     */
    String Ast::dmesg( StringList *args )
    {
        String s;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        switch( count(args) )
        {
            case 0:
                r[ "bootCause" ] = static_cast<int>( LowPower.bootCause() );
                r[ "clockMode" ] = static_cast<int>( LowPower.getClockMode() );
                r[ "current" ] = LowPower.getCurrent();
                r[ "voltage" ] = LowPower.getVoltage();
                r.prettyPrintTo( s );
                break;
            default:
                s = usage( "dmesg" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::reboot( StringList *args )
     *
     * Reboot the system.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::reboot( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                LowPower.reboot();
                break;
            default:
                s = usage( "reboot" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::sleep( StringList *args )
     *
     * Sleep (yield) this thread.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::sleep( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
            // Not work?
                LowPower.sleep( _atoui(0) );
                break;
            default:
                s = usage( "sleep [0-9]+" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::lteBegin( StringList *args )
     *
     * Power on the modem and start the network search. And register the modem
     * on the LTE network.
     *
     * @param[in] args Reference to arguments.
     * @return Network information if ready, Otherwise empty string.
     */
    String Ast::lteBegin( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 3:
                if( Lte.begin()!=LTE_SEARCHING )
                    s = lteBegin( 0 );
                else
                {
                    if( Lte.attach(_a(0).c_str(),_a(1).c_str(),_a(2).c_str())!=LTE_READY )
                        s = lteBegin( 0 );
                    else
                    {
                        StringList sl;
                        s = lteConfig( &sl );
                    }
                }
                break;
            default:
                s = usage( "lteBegin APN USERNAME PASSWORD" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::lteConfig( StringList *args )
     *
     * Return LTE network information.
     *
     * @param[in] args Reference to arguments.
     * @return Network information.
     */
    String Ast::lteConfig( StringList *args )
    {
        String s;
        IPAddress ip;
        char buf[ 128 ];
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        switch( count(args) )
        {
            case 0:
                r[ "carrier" ] = LteScanner.getCurrentCarrier();
                r[ "firmware" ] = LteModem.getFirmwareVersion();
                ip = Lte.getIPAddress();
                snprintf( buf, sizeof(buf), "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3] );
                r[ "ipAddress" ] = buf;
                r[ "imei" ] = LteModem.getIMEI();
                r[ "signalStrength" ] = LteScanner.getSignalStrength();
                r[ "status" ] = static_cast<int>( LteScanner.getStatus() );
                r.prettyPrintTo( s );
                break;
            default:
                s = usage( "lteConfig" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::lteEnd( StringList *args )
     *
     * Detach the modem from the LTE network and power off the LTE modem.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::lteEnd( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                if( Lte.getStatus()!=LTE_READY )
                    s = lteEnd( 0 );
                else
                    Lte.detach();
                    Lte.shutdown();
                break;
            default:
                s = usage( "lteEnd" );
                break;
        }

        return s;
    }
}
#endif
