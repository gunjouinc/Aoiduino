/******************************************************************************
**
** Copyright 2009-2023 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#ifdef ESP8266
/** Esp WiFi timeout */
#define ESP_WIFI_TIMEOUT 30000

#include "esp-esp8266.h"
/* Flash */
#include <FS.h>
FS *EspStorage = &SPIFFS;
/* RTC */
#include <time.h>
/* Ticker */
#include <Ticker.h>
Ticker ticker;
Ticker watchdog;
uint32_t watchdogSecond = 0;
uint32_t watchdogMills = 0;
/* WiFi */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
WiFiClient wifiClient;
WiFiClientSecure wifiClientSecure;

/** File mode - append */
#define _FILE_APPEND_ "a"
/** File mode - read */
#define _FILE_READ_ "r"
/** File mode - write */
#define _FILE_WRITE_ "w"
/** Flash root path */
#define _FLASH_ "/mnt/spif"

/**
 * @namespace AoiEsp
 * @brief Aoi esp classes.
 */
namespace AoiEsp
{
// Static variables.
    AoiBase::FunctionTable *Esp8266::m_functionTable = 0;
    /**
     * @fn Esp8266::Esp8266( void )
     *
     * Constructor. Member variables are initialized.
     */
    Esp8266::Esp8266( void )
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
            { ">", &Esp8266::create },
            { ">>", &Esp8266::append },
            { "cat", &Esp8266::read },
            { "cd", &Esp8266::cd },
            { "format", &Esp8266::format },
            { "ll", &Esp8266::ll },
            { "mkdir", &Esp8266::mkdir },
            { "pwd", &Esp8266::pwd },
            { "rm", &Esp8266::remove },
            { "rmdir", &Esp8266::rmdir },
            { "touch", &Esp8266::touch },
            /* LowPower */
            { "deepSleep", &Esp8266::deepSleep },
            { "dmesg", &Esp8266::dmesg },
            { "reboot", &Esp8266::restart },
            /* HTTP */
            { "httpBegin", &Esp8266::httpBegin },
            { "httpGet", &AoiUtil::Http::httpGet },
            { "httpPost", &Esp8266::httpPost },
            /* IR */
            { "irReceiveBegin", &AoiUtil::IRRemote::irReceiveBegin },
            { "irReceiveRaw", &AoiUtil::IRRemote::irReceiveRaw },
            { "irSendBegin", &AoiUtil::IRRemote::irSendBegin },
            { "irSendRaw", &AoiUtil::IRRemote::irSendRaw },
            /* RTC */
            { "date", &Esp8266::date },
            /* Servo */
            { "servoAttach", &AoiUtil::Servo_::servoAttach },
            { "servoBegin", &AoiUtil::Servo_::servoBegin },
            { "servoEnd", &AoiUtil::Servo_::servoEnd },
            { "servoWriteMicroseconds", &AoiUtil::Servo_::servoWriteMicroseconds },
            /* Watchdog */
            { "watchdogBegin", &Esp8266::watchdogBegin },
            { "watchdogEnd", &Esp8266::watchdogEnd },
            { "watchdogKick", &Esp8266::watchdogKick },
            { "watchdogTimeleft", &Esp8266::watchdogTimeleft },
            /* WiFi */
            { "ifconfig", &Esp8266::ifConfig },
            { "iwlist", &Esp8266::wifiScanNetworks },
            { "wifiBegin", &Esp8266::wifiBegin },
            { "wifiEnd", &Esp8266::wifiEnd },
            { "wifiRtc", &Esp8266::wifiRtc },
        // $ Please set your function to use.
            { "", 0 }
        };
    // Creates function table to avoid kernel panic.
        uint8_t c = sizeof(ftl) / sizeof(AoiBase::FunctionTable);
        m_functionTable = Arduino::functionTable( ftl, c );
    // Initalize library
        /* File */
        SPIFFS.begin();
        /* HTTP */
        http = &wifiClient;
    }
    /**
     * @fn Esp8266::~Esp8266( void )
     *
     * Destructor. Member variables are deleted.
     */
    Esp8266::~Esp8266( void )
    {
    }
    /**
     * @fn String Esp8266::className( void )
     *
     * @see bool AbstractBase::className( void )
     */
    String Esp8266::className( void )
    {
        return String( "Esp8266" );
    }
    /**
     * @fn bool Esp8266::isExist( const String &function )
     *
     * @see bool AbstractBase::isExist( const String &function )
     */
    bool Esp8266::isExist( const String &function )
    {
        return Arduino::isExist( function, m_functionTable );
    }
    /**
     * @fn String Esp8266::practice( StringList *args )
     *
     * @see String AbstractBase::practice( StringList *args )
     */
    String Esp8266::practice( StringList *args )
    {
        return Arduino::practice( args, m_functionTable );
    }
    /**
     * @fn String Esp8266::usages( void )
     *
     * @see String AbstractBase::usages( void )
     */
    String Esp8266::usages( void )
    {
        return Arduino::usages( m_functionTable );
    }
    /**
     * @fn StringList* Esp8266::rcScript( const String &index )
     *
     * @see StringList* AbstractBase::rcScript( const String &index )
     */
    StringList* Esp8266::rcScript( const String &index )
    {
        StringList *sl = 0;
        String s = appendRootPath( index );

        if( !EspStorage->exists(s) )
            return sl;

        File f = EspStorage->open( s, _FILE_READ_ );
        if( f )
        {
            String s = f.readString();
            f.close();
            sl = split( s, String(_lf) );
        }

        return sl;
    }
    /**
     * @fn String Esp8266::append( StringList *args )
     *
     * Append value on current device.
     *
     * @param[in] args Reference to arguments.
     * @return value string.
     */
    String Esp8266::append( StringList *args )
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
                File w = EspStorage->open( t, _FILE_WRITE_ );
                w.close();
            }
            File f = EspStorage->open( t, _FILE_APPEND_ );
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
            f = EspStorage->open( t, _FILE_READ_ );
            s = prettyPrintTo( "value", f.readString() );
            f.close();
         }

         return s;
    }
    /**
     * @fn String Esp8266::cd( StringList *args )
     *
     * Change device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp8266::cd( StringList *args )
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
     * @fn String Esp8266::create( StringList *args )
     *
     * Create value on current device.
     *
     * @param[in] args Reference to arguments.
     * @return value string.
     */
    String Esp8266::create( StringList *args )
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
     * @fn String Esp8266::format( StringList *args )
     *
     * Format file device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp8266::format( StringList *args )
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
     * @fn String Esp8266::ll( StringList *args )
     *
     * Return file detail list in current device.
     *
     * @param[in] args Reference to arguments.
     * @return Current device.
     */
    String Esp8266::ll( StringList *args )
    {
        String s, p, root;
        DynamicJsonBuffer json;
        JsonArray &r = json.createArray();
        Dir d;
        File f;

        switch( count(args) )
        {
            case 1:
                root = _a( 0 );
            case 0:
            // Root path
                root = appendRootPath( root );
                d = EspStorage->openDir( root );
                p = root;
            // File info to JSON
                while( d.next() )
                {
                    File f = d.openFile( "r" );
                    JsonObject &o = r.createNestedObject();
                    o[ "type" ] = f.isDirectory() ? "d" : "-";
                    o[ "name" ] = String(f.name()).substring( p.length() );
                    o[ "size" ] = f.size();
                    f.close();
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
     * @fn String Esp8266::mkdir( StringList *args )
     *
     * Make directory on current device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp8266::mkdir( StringList *args )
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
     * @fn String Esp8266::pwd( StringList *args )
     *
     * Return current device.
     *
     * @param[in] args Reference to arguments.
     * @return Current device.
     */
    String Esp8266::pwd( StringList *args )
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
     * @fn String Esp8266::read( StringList *args )
     *
     * Return file content on current device.
     *
     * @param[in] args Reference to arguments.
     * @return File content.
     */
    String Esp8266::read( StringList *args )
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
                    f = EspStorage->open( t, _FILE_READ_ );
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
     * @fn String Esp8266::remove( StringList *args )
     *
     * Remove file on current device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp8266::remove( StringList *args )
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
     * @fn String Esp8266::rmdir( StringList *args )
     *
     * Remove directory on current device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp8266::rmdir( StringList *args )
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
     * @fn String Esp8266::httpBegin( StringList *args )
     *
     * Initalize https certs.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp8266::httpBegin( StringList *args )
    {
        String s, t;
        File rc, cc, pk;

        switch( count(args) )
        {
            case 0:
                http = &wifiClient;
                break;
/*
 * Feature removed due to underlying issue.
 *
            case 3:
                http = &wifiClientSecure;
            // RootCA
                rc = EspStorage->open( appendRootPath(_a(0)), _FILE_READ_ );
                t = rc.readString();
                wifiClientSecure.setCACert( (uint8_t*)t.c_str(), t.length() );
                rc.close();
            // Client certificate
                cc = EspStorage->open( appendRootPath(_a(1)), _FILE_READ_ );
                t = cc.readString();
                wifiClientSecure.setCertificate( (uint8_t*)t.c_str(), t.length() );
                cc.close();
            // Client private key
                pk = EspStorage->open( appendRootPath(_a(2)), _FILE_READ_ );
                t = pk.readString();
                wifiClientSecure.setPrivateKey( (uint8_t*)t.c_str(), t.length() );
                pk.close();
                break;
            default:
                s = usage( "httpBegin CACert Certificate PrivateKey" );
*/
            default:
                s = usage( "httpBegin" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp8266::httpPost( StringList *args )
     *
     * Send HTTP POST to server.
     *
     * @param[in] args Reference to arguments.
     * @return Recieved content.
     */
    String Esp8266::httpPost( StringList *args )
    {
        String s, t, header, footer;
        File f;
        int size = 0;
        String host;
        int port = 80;
        int timeout = 30 * 1000;
        uint8_t *buf = 0;

        switch( count(args) )
        {
            case 5:
                timeout = _atoi( 4 ) * 1000;
            case 4:
                port = _atoi( 3 );
            case 3:
                t = appendRootPath( _a(2) );
                if( !EspStorage->exists(t) )
                    return AoiUtil::Http::httpPost( args );
            // Request body
                header = requestBodyHeaderInPut( STR_BOUNDARY, STR_AOIDUINO, t,
                                                 &size );
                footer = requestBodyFooterInPut( STR_BOUNDARY );
                size += header.length() + footer.length();
            // POST
                host = _a( 0 );
                if( !http->connect(host.c_str(),port) )
                    return httpPost( 0 );
                http->println( "POST "+_a(1)+" HTTP/1.0" );
                http->println( "Host: " + host );
                http->println( "User-Agent: " + String(STR_USER_AGENT) );
                http->print( "Content-Type: multipart/form-data; " );
                http->println( "boundary=\""+String(STR_BOUNDARY)+"\"" );
                http->println( "Content-Length: "+String(size) );
                http->println( "Connection: close" );
                http->println();
                http->print( header );
              // Upload file
                f = EspStorage->open( t, _FILE_READ_ );
                buf = new uint8_t[ _AOIUTIL_HTTP_BUFFER_SIZE_ ];
                while( f.available() )
                {
                    size = f.read( buf, _AOIUTIL_HTTP_BUFFER_SIZE_ );
                    http->write( buf, size );
                }
                delete [] buf;
                f.close();
                http->print( footer );
            // Response
                s = response( timeout );
                s = prettyPrintTo( "value", s );
                break;
            default:
                s = usage( "httpPost host path (file|text) (port timeout)?" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp8266::touch( StringList *args )
     *
     * Create empty file on current device.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp8266::touch( StringList *args )
    {
        String s, t;
        File f;

        switch( count(args) )
        {
            case 1:
                t = appendRootPath( _a(0) );
                f = EspStorage->open( t, _FILE_WRITE_ );
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
     * @fn String Esp8266::deepSleep( StringList *args )
     *
     * Enter the deep sleep state.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp8266::deepSleep( StringList *args )
    {
        String s;
        RFMode mode;

        switch( count(args) )
        {
            case 0:
                ESP.deepSleep( 0 );
                break;
            case 1:
            // Micro second.
                mode = WAKE_RF_DEFAULT;
                ESP.deepSleep( _atoi(0) * 1000 * 1000, mode );
                break;
            default:
                s = usage( "deepSleep [0-9]*" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp8266::dmesg( StringList *args )
     *
     * Returns system information.
     *
     * @param[in] args Reference to arguments.
     * @return System information.
     */
    String Esp8266::dmesg( StringList *args )
    {
        String s;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        switch( count(args) )
        {
            case 0:
                r[ "chipId" ] = String( ESP.getChipId(), HEX );
                r[ "coreVersion" ] = ESP.getCoreVersion();
                r[ "cpuFreqMHz" ] = ESP.getCpuFreqMHz();
                r[ "flashChipId" ] = String( ESP.getFlashChipId(), HEX );
                r[ "flashChipSize" ] = ESP.getFlashChipSize();
                r[ "freeHeap" ] = ESP.getFreeHeap();
                r[ "freeSketchSpace" ] = ESP.getFreeSketchSpace();
                r[ "resetReason" ] = ESP.getResetReason();
                r[ "sdkVersion" ] = ESP.getSdkVersion();
                r[ "sketchSize" ] = ESP.getSketchSize();
                r.prettyPrintTo( s );
                break;
            default:
                s = usage( "dmesg" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp8266::restart( StringList *args )
     *
     * Reboot the system.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp8266::restart( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                ESP.restart();
                break;
            case 1:
                ticker.once_ms( _atoi(0), reboot );
                break;
            default:
                s = usage( "reboot ([0-9]+)" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp8266::date( StringList *args )
     *
     * Print date.
     *
     * @param[in] args Reference to arguments.
     * @return date string.
     */
    String Esp8266::date( StringList *args )
    {
        String s;
        uint8_t size = 20;
        char *buf = NULL;
        time_t t;
        struct tm *rtc;
        struct timeval tv;

        switch( count(args) )
        {
            case 0:
                t = time( NULL );
                rtc = localtime( &t );
                buf = new char[ size ];
                snprintf( buf, size,
                          "%04d-%02d-%02dT%02d:%02d:%02d",
                          rtc->tm_year+1900, rtc->tm_mon+1, rtc->tm_mday,
                          rtc->tm_hour, rtc->tm_min, rtc->tm_sec );
                s = prettyPrintTo( "value", buf );
                delete [] buf;
                break;
            case 1:
                tv.tv_sec = _atoul( 0 );
                settimeofday( &tv, NULL );
                break;
            default:
                s = usage( "date (unixtime)?" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp8266::watchdogBegin( StringList *args )
     *
     * Initialize the Watchdog and start to check timer(mesc).
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp8266::watchdogBegin( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
                watchdog.once_ms( _atoi(0), reboot );
                watchdogSecond = _atoi(0);
                watchdogMills = ::millis();
                break;
            default:
                s = usage( "watchdogBegin [0-9]+" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp8266::watchdogEnd( StringList *args )
     *
     * Stop to check timer for avoid bite watchdog.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp8266::watchdogEnd( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                watchdog.detach();
                watchdogSecond = 0;
                watchdogMills = 0;
                break;
            default:
                s = usage( "watchdogEnd" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::watchdogKick( StringList *args )
     *
     * Kick to watchdog for notify keep alive.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp8266::watchdogKick( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                watchdog.detach();
                watchdog.once_ms( watchdogSecond, reboot );
                watchdogMills = ::millis();
                break;
            default:
                s = usage( "watchdogKick" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp8266::watchdogTimeleft( StringList *args )
     *
     * Get a remain time for bite watchdog.
     *
     * @param[in] args Reference to arguments.
     * @return Remain time to expire timeout(mesc).
     */
    String Esp8266::watchdogTimeleft( StringList *args )
    {
        String s;
        uint32_t i = 0;

        switch( count(args) )
        {
            case 0:
                i = watchdogSecond - (::millis()-watchdogMills);
                s = prettyPrintTo( "value" , i );
                break;
            default:
                s = usage( "watchdogTimeleft" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp8266::ifConfig( StringList *args )
     *
     * Show network information.
     *
     * @param[in] args Reference to arguments.
     * @return Network information.
     */
    String Esp8266::ifConfig( StringList *args )
    {
        String s;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        switch( count(args) )
        {
            case 0:
                r[ "ipAddress" ] = WiFi.localIP().toString();
                r[ "subnetMask" ] = WiFi.subnetMask().toString();
                r[ "gatewayIp" ] = WiFi.gatewayIP().toString();
                r[ "macAddress" ] = WiFi.macAddress();
                r[ "dnsIP1" ] = WiFi.dnsIP( 0 ).toString();
                r[ "dnsIP2" ] = WiFi.dnsIP( 1 ).toString();
                r[ "softAP" ] = WiFi.softAPIP().toString();
                r.prettyPrintTo( s );
                break;
            default:
                s = usage( "ifconfig" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp8266::wifiScanNetworks( StringList *args )
     *
     * Scan wifi networks.
     *
     * @param[in] args Reference to arguments.
     * @return Returns SSID, RSSI and Encryption type.
     */
    String Esp8266::wifiScanNetworks( StringList *args )
    {
        String s;
        int i = 0;
        DynamicJsonBuffer json;
        JsonArray &r = json.createArray();

        switch( count(args) )
        {
            case 0:
                i = WiFi.scanNetworks();
                if( !i )
                    s = STR_NO_NETWORKS_FOUND;
                else
                {
                    for( int j=0; j<i; j++ )
                    {
                        JsonObject &o = r.createNestedObject();
                        o[ "ssid" ] = WiFi.SSID( j );
                        o[ "rssi" ] = WiFi.RSSI( j );
                    // Sets encription type.
                        String t = "";
                        switch( WiFi.encryptionType(j) )
                        {
                            case 2:
                                t = "TKIP(WPA)";
                                break;
                            case 5:
                                t = "WEP";
                                break;
                            case 4:
                                t = "CCMP(WPA)";
                                break;
                            case 7:
                                t = "None";
                                break;
                            case 8:
                                t = "Auto";
                                break;
                            default:
                                t = "Other";
                                break;
                        }
                        o[ "type" ] = t;
                    }
                    r.prettyPrintTo( s );
                }
                break;
            default:
                s = usage( "iwlist" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp8266::wifiBegin( StringList *args )
     *
     * Connect to wireless network.
     *
     * @param[in] args Reference to arguments.
     * @return Wireless ip address, Otherwise error string.
     */
    String Esp8266::wifiBegin( StringList *args )
    {
        String s;
        unsigned long i = 0;

        switch( count(args) )
        {
            case 2:
            // Start
                WiFi.disconnect();
                WiFi.mode( WIFI_STA );
                WiFi.begin( _a(0).c_str(), _a(1).c_str() );
            // Waiting
                i = ::millis();
                while( WiFi.status()!=WL_CONNECTED && (::millis()-i)<ESP_WIFI_TIMEOUT )
                    ::delay( 500 );
            // Result
                if( WiFi.status()!=WL_CONNECTED )
                    s = STR_CANT_CONNECT_TO_WIRELESS_NETWORK;
                else
                {
                    StringList sl;
                    s = ifConfig( &sl );
                }
                break;
            default:
                s = usage( "wifiBegin ssid password" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp8266::wifiEnd( StringList *args )
     *
     * Detach from wireless network.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Esp8266::wifiEnd( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                if( WiFi.status()!=WL_CONNECTED )
                    s = wifiEnd( 0 );
                else
                    WiFi.disconnect();
                break;
            default:
                s = usage( "wifiEnd" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp8266::wifiRtc( StringList *args )
     *
     * Get rtc from the WiFi network.
     *
     * @param[in] args Reference to arguments.
     * @return unixtime string.
     */
    String Esp8266::wifiRtc( StringList *args )
    {
        String s;
        time_t now;

        switch( count(args) )
        {
            case 0:
                if( WiFi.status()!=WL_CONNECTED )
                    s = wifiRtc( 0 );
                else
                {
                    configTzTime( "JST-9", "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp" );
                    time( &now );
                    s = String( now );
                    s = prettyPrintTo( "value", s );
                }
                break;
            default:
                s = usage( "wifiRtc" );
                break;
        }

        return s;
    }
    /**
     * @fn String Esp8266::appendRootPath( const String &path )
     *
     * Append root path ("/") if need.
     *
     * @return Path include root.
     */
    String Esp8266::appendRootPath( const String &path )
    {
        String s = path;
        String r = "/";

        if( s.indexOf(r) )
            s = r + s;

        return s;
    }
    /**
     * @fn void Esp8266::reboot( void )
     *
     * Reboot the system.
     */
    void Esp8266::reboot( void )
    {
        ESP.restart();
    }
    /**
     * @fn String Esp8266::requestBodyHeaderInPut( const String &value )
     *
     * Return request body header in HTTP PUT.
     *
     * @param[in] boundary Boundary string.
     * @param[in] name Content-Disposition: name attribute string.
     * @param[in] value Putted value.
     * @param[in/out] size Putted value size. If value is file, File size is used.
     * @return Request body header string in HTTP PUT.
     */
    String Esp8266::requestBodyHeaderInPut( const String &boundary, const String &name, const String &value, int *size )
    {
        String s;
        String t = appendRootPath( value );

    // If file is exitst, use file size
        if( !EspStorage->exists(t) )
            s = AoiUtil::Http::requestBodyHeaderInPut( boundary, name, value, size );
        else
        {
            s += "--" + boundary + "\r\n";
            s += "Content-Disposition: form-data; name=\"" + name + "\";";
            s += " filename=\"" + value + "\"\r\n";
            s += "Content-Type: application/octet-stream\r\n";
            s += "Content-Transfer-Encoding: binary\r\n";
            File f = EspStorage->open( t, _FILE_READ_ );
            *size = f.size();
            f.close();
            s += "\r\n";
        }

        return s;
    }
}
#endif
