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
#include <RTC.h>
#include <Watchdog.h>
/* LTE */
#include <LTE.h>
LTE Lte;
LTETLSClient LteTlsClient;
LTEScanner LteScanner;
LTEModemVerification LteModem;
/* MP */
#include <MP.h>
/* MQTT */
#include <ArduinoMqttClient.h>
MqttClient MqttTlsClient( LteTlsClient );

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
            { "echo", &Arduino::echo },
            { "led", &Ast::led },
            { "micros", &Arduino::micros },
            { "millis", &Arduino::millis },
            { "noTone", &Arduino::noTone },
            { "pinMode", &Arduino::pinMode },
            { "tone", &Arduino::tone },
            /* Camera */
            { "cameraBegin", &Ast::cameraBegin },
            { "cameraEnd", &Ast::cameraEnd },
            { "cameraAutoWhiteBalanceMode", &Ast::cameraSetAutoWhiteBalanceMode },
            { "cameraColorEffect", &Ast::cameraSetColorEffect },
            { "cameraPictureFormat", &Ast::cameraSetStillPictureImageFormat },
            { "cameraTakePicture", &Ast::cameraTakePicture },
            /* File */
            { ">", &Ast::create },
            { ">>", &Ast::append },
            { "beginUsbMsc", &Ast::beginUsbMsc },
            { "cat", &Ast::read },
            { "cd", &Ast::cd },
            { "endUsbMsc", &Ast::endUsbMsc },
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
            /* MQTT */
            { "mqttBegin", &Ast::mqttBegin },
            { "mqttConnect", &Ast::mqttConnect },
            { "mqttPublish", &Ast::mqttPublish },
            { "mqttPoll", &Ast::mqttPoll },
            { "mqttSubscribe", &Ast::mqttSubscribe },
            { "mqttUnSubscribe", &Ast::mqttUnSubscribe },
        // $ Please set your function to use.
            { "", 0 }
        };
    // Creates function table to avoid kernel panic.
        uint8_t c = sizeof(ftl) / sizeof(AoiBase::FunctionTable);
        m_functionTable = Arduino::functionTable( ftl, c );
    // Initalize library
        /* LowPower */
        LowPower.begin();
        RTC.begin();
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
     * @fn StringList* Ast::rcScript( const String &index )
     *
     * @see StringList* AbstractBase::rcScript( const String &index )
     */
    StringList* Ast::rcScript( const String &index )
    {
        StringList *sl = 0;

        if( !AstStorage->exists(index) )
            return sl;

        File f = AstStorage->open( index, FILE_READ );
        if( f )
        {
            String s = f.readString();
            f.close();
            sl = split( s, String(_lf) );
        }

        return sl;
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
     * @fn String Ast::cameraBegin( StringList *args )
     *
     * Begin camera function.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::cameraBegin( StringList *args )
    {
        String s;
        CAM_VIDEO_FPS fps;
        int width, height;
        CAM_IMAGE_PIX_FMT format;
        CamErr r;

        switch( count(args) )
        {
            case 4:
                if( !fpsFromString(_a(1),&fps) )
                    return cameraBegin( 0 );
                if( !sizeFromString(_a(2),&width,&height) )
                    return cameraBegin( 0 );
                if( !formatFromString(_a(3),&format) )
                    return cameraBegin( 0 );

                r = theCamera.begin( _atoi(0), fps, width, height, format );
                if( r!=CAM_ERR_SUCCESS )
                    return cameraBegin( 0 );
                break;
            default:
                s = usage( "cameraBegin buffNum (5|6|7.5|15|30|120) (QVGA|VGA|HD|QUADVGA|FULLHD|5M|3M) (RGB565|YUV422|JPG|GRAY|NONE)" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::cameraEnd( StringList *args )
     *
     * End camera function.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::cameraEnd( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                theCamera.end();
                break;
            default:
                s = usage( "cameraEnd" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::cameraSetAutoWhiteBalanceMode( StringList *args )
     *
     * Set camera auto white balance mode.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::cameraSetAutoWhiteBalanceMode( StringList *args )
    {
        String s;
        CAM_WHITE_BALANCE wb;
        CamErr r;

        switch( count(args) )
        {
            case 1:
                if( !whiteBalanceFromString(_a(0),&wb) )
                    return cameraSetAutoWhiteBalanceMode( 0 );

                r = theCamera.setAutoWhiteBalanceMode( wb );
                if( r!=CAM_ERR_SUCCESS )
                    return cameraSetAutoWhiteBalanceMode( 0 );
                break;
            default:
                s = usage( "cameraAutoWhiteBalanceMode (AUTO|INCANDESCENT|FLUORESCENT|DAYLIGHT|FLASH|CLOUDY|SHADE)" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::cameraSetColorEffect( StringList *args )
     *
     * Set camera color effect.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::cameraSetColorEffect( StringList *args )
    {
        String s;
        CAM_COLOR_FX effect;
        CamErr r;

        switch( count(args) )
        {
            case 1:
                if( !effectFromString(_a(0),&effect) )
                    return cameraSetColorEffect( 0 );

                r = theCamera.setColorEffect( effect );
                if( r!=CAM_ERR_SUCCESS )
                    return cameraSetColorEffect( 0 );
                break;
            default:
                s = usage( "cameraColorEffect (NONE|BW|SEPIA|NEGATIVE|EMBOSS|SKETCH|BLUE|GREEN|WHITEN|VIVID|AQUA|FREEZE|SILHOUETTE|SOLARIZATION|ANTIQUE|CBCR|PASTEL)" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::cameraSetStillPictureImageFormat( StringList *args )
     *
     * Set camera still picture image format.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::cameraSetStillPictureImageFormat( StringList *args )
    {
        String s;
        int width, height;
        CAM_IMAGE_PIX_FMT format;
        CamErr r;

        switch( count(args) )
        {
            case 2:
                if( !sizeFromString(_a(0),&width,&height) )
                    return cameraSetStillPictureImageFormat( 0 );
                if( !formatFromString(_a(1),&format) )
                    return cameraSetStillPictureImageFormat( 0 );

                r = theCamera.setStillPictureImageFormat( width, height, format );
                if( r!=CAM_ERR_SUCCESS )
                    return cameraSetStillPictureImageFormat( 0 );
                break;
            default:
                s = usage( "cameraPictureFormat (QVGA|VGA|HD|QUADVGA|FULLHD|5M|3M) (RGB565|YUV422|JPG|GRAY|NONE)" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::cameraTakePicture( StringList *args )
     *
     * Take picture.
     *
     * @param[in] args Reference to arguments.
     * @return Taked picture information.
     */
    String Ast::cameraTakePicture( StringList *args )
    {
        String s;
        CamImage image;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();
        File f;

        switch( count(args) )
        {
            case 1:
                image = theCamera.takePicture();
                if( !image.isAvailable() )
                    return cameraTakePicture( 0 );
                r[ "format" ] = image.getPixFormat();
                r[ "height" ] = image.getHeight();
                r[ "size" ] = image.getImgSize();
                r[ "width" ] = image.getWidth();
                r.prettyPrintTo( s );
            // Save to current storage
                if( AstStorage->exists(_a(0)) )
                    AstStorage->remove( _a(0) );
                f = AstStorage->open( _a(0), FILE_WRITE );
                f.write( image.getImgBuff(), image.getImgSize() );
                f.close();
                break;
            default:
                s = usage( "cameraTakePicture file" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::append( StringList *args )
     *
     * Append value on current device.
     *
     * @param[in] args Reference to arguments.
     * @return value string.
     */
    String Ast::append( StringList *args )
    {
        String s;
        int c = count( args );

        if( c<2 )
            s = usage( ">> file .+" );
        else
        {
            File f = AstStorage->open( _a(0), FILE_WRITE );
            if( 0<f.size() )
                f.print( _lf );
            for( int i=1; i<c; i++ )
            {
                if( 1<i )
                    f.print( STR_SPACE );
                f.print( _a(i) );
            }
            f.seek( 0 );
            s = prettyPrintTo( "value", f.readString() );
            f.close();
         }

         return s;
    }
    /**
     * @fn String Ast::beginUsbMsc( StringList *args )
     *
     * Start USB Mass Storage Class.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::beginUsbMsc( StringList *args )
    {
        String s;
        String path;

        switch( count(args) )
        {
            case 1:
                path = _a( 0 );
                if( path==_EMMC_ )
                {
                    eMMC.begin();
                    eMMC.beginUsbMsc();
                }
                else if( path==_SD_ )
                {
                    AstSD.begin();
                    AstSD.beginUsbMsc();
                }
                else
                    s = beginUsbMsc( 0 );
                break;
            default:
                s = usage( "beginUsbMsc ("+String(_EMMC_)
                                      +"|"+String(_SD_)+")" );
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
                {
                    eMMC.begin();
                    AstStorage = &eMMC;
                }
                else if( (path==_FLASH_) || (path=="/") )
                    AstStorage = &Flash;
                else if( path==_SD_ )
                {
                    AstSD.begin();
                    AstStorage = &AstSD;
                }
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
     * @fn String Ast::create( StringList *args )
     *
     * Create value on current device.
     *
     * @param[in] args Reference to arguments.
     * @return value string.
     */
    String Ast::create( StringList *args )
    {
        String s;
        int c = count( args );

        if( c<2 )
            s = usage( "> file .+" );
        else
        {
            if( AstStorage->exists(_a(0)) )
                AstStorage->remove( _a(0) );
            s = append( args );
        }

        return s;
    }
    /**
     * @fn String Ast::endUsbMsc( StringList *args )
     *
     * Stop USB Mass Storage Class.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::endUsbMsc( StringList *args )
    {
        String s;
        String path;

        switch( count(args) )
        {
            case 1:
                path = _a( 0 );
                if( path==_EMMC_ )
                    eMMC.endUsbMsc();
                else if( path==_SD_ )
                    AstSD.endUsbMsc();
                else
                    s = endUsbMsc( 0 );
                break;
            default:
                s = usage( "endUsbMsc ("+String(_EMMC_)
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
                        return ll( 0 );
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
                s = usage( "ll( path)?" );
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
        SpNavData nav;
        char buf[ 128 ];
        int diff;
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
                        diff = nav.longitude / (360/24);
                        r[ "numSatellites" ] = (int)nav.numSatellites;
                        r[ "fixMode" ] = (nav.posFixMode==FixInvalid) ? "No-Fix" : "Fix";
                        snprintf( buf, sizeof(buf),
                                  "%04d-%02d-%02dT%02d:%02d:%02d.%06d+%02d:00",
                                  nav.time.year, nav.time.month, nav.time.day,
                                  nav.time.hour, nav.time.minute, nav.time.sec, nav.time.usec,
                                  diff );
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
        int used, free, largest;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        switch( count(args) )
        {
            case 0:
                r[ "bootCause" ] = static_cast<int>( LowPower.bootCause() );
                r[ "clockMode" ] = static_cast<int>( LowPower.getClockMode() );
                r[ "current" ] = LowPower.getCurrent();
                MP.GetMemoryInfo( used, free, largest );
                r[ "memoryFree" ] = free;
                r[ "memoryLargestFree" ] = largest;
                r[ "memoryUsed" ] = used;
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
            case 1:
                Watchdog.begin();
                Watchdog.start( _atoi(0) );
                break;
            default:
                s = usage( "reboot [0-9]*" );
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
            case 0:
                if( Lte.begin()!=LTE_SEARCHING )
                    s = lteBegin( 0 );
                break;
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
                s = usage( "lteBegin (APN USERNAME PASSWORD)?" );
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
                {
                    Lte.detach();
                    Lte.shutdown();
                }
                break;
            default:
                s = usage( "lteEnd" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::mqttBegin( StringList *args )
     *
     * Initalize mqtt certs.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::mqttBegin( StringList *args )
    {
        String s;
        File rc, cc, pk;

        switch( count(args) )
        {
            case 3:
            // RootCA
                rc = AstStorage->open( _a(0), FILE_READ );
                LteTlsClient.setCACert( rc, rc.available() );
                rc.close();
            // Client certificate
                cc = AstStorage->open( _a(1), FILE_READ );
                LteTlsClient.setCertificate( cc, cc.available() );
                cc.close();
            // Client private key
                pk = AstStorage->open( _a(2), FILE_READ );
                LteTlsClient.setPrivateKey( pk, pk.available() );
                pk.close();
                break;
            default:
                s = usage( "mqttBegin CACert Certificate PrivateKey" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::mqttConnect( StringList *args )
     *
     * Connect to mqtt broker.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::mqttConnect( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 2:
                if( !MqttTlsClient.connect(_a(0).c_str(),_atoi(1)) )
                    return mqttConnect( 0 );
                break;
            default:
                s = usage( "mqttConnect broker port" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::mqttPublish( StringList *args )
     *
     * Publish message to topic.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::mqttPublish( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 3:
                if( !MqttTlsClient.beginMessage(_a(0),false,_atoui(1)) )
                    return mqttPublish( 0 );
                MqttTlsClient.print( _a(2) );
                MqttTlsClient.endMessage();
                break;
            default:
                s = usage( "mqttPublish topic [0-2] message" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::mqttPoll( StringList *args )
     *
     * Regularly to allow the library to receive MQTT messages and
     * send MQTT keep alives which avoids being disconnected by the broker.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::mqttPoll( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                MqttTlsClient.poll();
                break;
            default:
                s = usage( "mqttPoll" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::mqttSubscribe( StringList *args )
     *
     * Subscribe message from topic.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::mqttSubscribe( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 2:
                MqttTlsClient.onMessage( mqttMessage );
                MqttTlsClient.subscribe( _a(0), _atoui(1) );
                break;
            default:
                s = usage( "mqttSubscribe topic [0-2]" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::mqttUnSubscribe( StringList *args )
     *
     * Unsubscribe message from topic.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::mqttUnSubscribe( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
                MqttTlsClient.unsubscribe( _a(0) );
                break;
            default:
                s = usage( "mqttUnSubscribe topic" );
                break;
        }

        return s;
    }
    /**
     * @fn bool Ast::effectFromString( const String &value, CAM_COLOR_FX *effect )
     *
     * Return CAM_COLOR_FX from string.
     *
     * @param[in] value effect type string like "SEPIA".
     * @param[in,out] effect reference to CAM_COLOR_FX.
     * @return Return true if value is valid, Otherwise return false.
     */
    bool Ast::effectFromString( const String &value, CAM_COLOR_FX *effect )
    {
        bool b = true;

        if( value=="NONE" )
            *effect = CAM_COLOR_FX_NONE;
        else if( value=="BW" )
            *effect = CAM_COLOR_FX_BW;
        else if( value=="SEPIA" )
            *effect = CAM_COLOR_FX_SEPIA;
        else if( value=="NEGATIVE" )
            *effect = CAM_COLOR_FX_NEGATIVE;
        else if( value=="EMBOSS" )
            *effect = CAM_COLOR_FX_EMBOSS;
        else if( value=="SKETCH" )
            *effect = CAM_COLOR_FX_SKETCH;
        else if( value=="BLUE" )
            *effect = CAM_COLOR_FX_SKY_BLUE;
        else if( value=="GREEN" )
            *effect = CAM_COLOR_FX_GRASS_GREEN;
        else if( value=="WHITEN" )
            *effect = CAM_COLOR_FX_SKIN_WHITEN;
        else if( value=="VIVID" )
            *effect = CAM_COLOR_FX_VIVID;
        else if( value=="AQUA" )
            *effect = CAM_COLOR_FX_AQUA;
        else if( value=="FREEZE" )
            *effect = CAM_COLOR_FX_ART_FREEZE;
        else if( value=="SILHOUETTE" )
            *effect = CAM_COLOR_FX_SILHOUETTE;
        else if( value=="SOLARIZATION" )
            *effect = CAM_COLOR_FX_SOLARIZATION;
        else if( value=="ANTIQUE" )
            *effect = CAM_COLOR_FX_ANTIQUE;
        else if( value=="CBCR" )
            *effect = CAM_COLOR_FX_SET_CBCR;
        else if( value=="PASTEL" )
            *effect = CAM_COLOR_FX_PASTEL;
        else
            b = false;

        return b;
    }
    /**
     * @fn bool Ast::formatFromString( const String &value, CAM_IMAGE_PIX_FMT *format )
     *
     * Return CAM_IMAGE_PIX_FMT from string.
     *
     * @param[in] value Format type string like "YUV422".
     * @param[in,out] format reference to CAM_IMAGE_PIX_FMT.
     * @return Return true if value is valid, Otherwise return false.
     */
    bool Ast::formatFromString( const String &value, CAM_IMAGE_PIX_FMT *format )
    {
        bool b = true;

        if( value=="RGB565" )
            *format = CAM_IMAGE_PIX_FMT_RGB565;
        else if( value=="YUV422" )
            *format = CAM_IMAGE_PIX_FMT_YUV422;
        else if( value=="JPG" )
            *format = CAM_IMAGE_PIX_FMT_JPG;
        else if( value=="GRAY" )
            *format = CAM_IMAGE_PIX_FMT_GRAY;
        else if( value=="NONE" )
            *format = CAM_IMAGE_PIX_FMT_NONE;
        else
            b = false;

        return b;
    }
    /**
     * @fn bool Ast::fpsFromString( const String &value, CAM_VIDEO_FPS *fps )
     *
     * Return CAM_VIDEO_FPS from string.
     *
     * @param[in] value fps type string like "30".
     * @param[in,out] fps reference to CAM_VIDEO_FPS.
     * @return Return true if value is valid, Otherwise return false.
     */
    bool Ast::fpsFromString( const String &value, CAM_VIDEO_FPS *fps )
    {
        bool b = true;

        if( value=="NONE" )
            *fps = CAM_VIDEO_FPS_NONE;
        else if( value=="5" )
            *fps = CAM_VIDEO_FPS_5;
        else if( value=="6" )
            *fps = CAM_VIDEO_FPS_6;
        else if( value=="7.5" )
            *fps = CAM_VIDEO_FPS_7_5;
        else if( value=="15" )
            *fps = CAM_VIDEO_FPS_15;
        else if( value=="30" )
            *fps = CAM_VIDEO_FPS_30;
        else if( value=="60" )
            *fps = CAM_VIDEO_FPS_60;
        else if( value=="120" )
            *fps = CAM_VIDEO_FPS_120;
        else
            b = false;

        return b;
    }
    /**
     * @fn bool Ast::whiteBalanceFromString( const String &value, CAM_WHITE_BALANCE *wb )
     *
     * Return CAM_WHITE_BALANCE from string.
     *
     * @param[in] value white balance type string like "AUTO".
     * @param[in,out] wb reference to CAM_WHITE_BALANCE.
     * @return Return true if value is valid, Otherwise return false.
     */
    bool Ast::whiteBalanceFromString( const String &value, CAM_WHITE_BALANCE *wb )
    {
        bool b = true;

        if( value=="AUTO" )
            *wb = CAM_WHITE_BALANCE_AUTO;
        else if( value=="5" )
            *wb = CAM_WHITE_BALANCE_INCANDESCENT;
        else if( value=="INCANDESCENT" )
            *wb = CAM_WHITE_BALANCE_FLUORESCENT;
        else if( value=="DAYLIGHT" )
            *wb = CAM_WHITE_BALANCE_DAYLIGHT;
        else if( value=="FLASH" )
            *wb = CAM_WHITE_BALANCE_FLASH;
        else if( value=="CLOUDY" )
            *wb = CAM_WHITE_BALANCE_CLOUDY;
        else if( value=="SHADE" )
            *wb = CAM_WHITE_BALANCE_SHADE;
        else
            b = false;

        return b;
    }
    /**
     * @fn bool Ast::sizeFromString( const String &value, int *width, int *height )
     *
     * Return size (width, height) from string.
     *
     * @param[in] value size type string like "QVGA".
     * @param[in,out] width reference to width.
     * @param[in,out] height reference to height.
     * @return Return true if value is valid, Otherwise return false.
     */
    bool Ast::sizeFromString( const String &value, int *width, int *height )
    {
        bool b = true;

        if( value=="QVGA" )
            { *width = CAM_IMGSIZE_QVGA_H; *height = CAM_IMGSIZE_QVGA_V; }
        else if( value=="VGA" )
            { *width = CAM_IMGSIZE_VGA_H; *height = CAM_IMGSIZE_VGA_V; }
        else if( value=="HD" )
            { *width = CAM_IMGSIZE_HD_H; *height = CAM_IMGSIZE_HD_V; }
        else if( value=="QUADVGA" )
            { *width = CAM_IMGSIZE_QUADVGA_H; *height = CAM_IMGSIZE_QUADVGA_V; }
        else if( value=="FULLHD" )
            { *width = CAM_IMGSIZE_FULLHD_H; *height = CAM_IMGSIZE_FULLHD_V; }
        else if( value=="5M" )
            { *width = CAM_IMGSIZE_5M_H; *height = CAM_IMGSIZE_5M_V; }
        else if( value=="3M" )
            { *width = CAM_IMGSIZE_3M_H; *height = CAM_IMGSIZE_3M_V; }
        else
            b = false;

        return b;
    }
    /**
     * @fn void Ast::mqttMessage( int messageSize )
     *
     * Receive message from topic.
     *
     * @param[in] message size.
     */
    void Ast::mqttMessage( int messageSize )
    {
        while( MqttTlsClient.available() )
            Serial.print( (char)MqttTlsClient.read() );
      Serial.println();
    }
}
#endif
