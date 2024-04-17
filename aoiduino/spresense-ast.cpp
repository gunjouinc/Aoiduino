/******************************************************************************
**
** Copyright 2009-2024 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#ifdef ARDUINO_spresense_ast
#include "spresense-ast.h"
/* Audio */
AudioClass *theAudio = AudioClass::getInstance();
#define PLAYER_BUFFER_SIZE 32768
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
#include <arch/board/board.h>
#include <RTC.h>
#include <Watchdog.h>
/* LTE */
#include <LTE.h>
LTE Lte;
LTEClient LteClient;
LTETLSClient LteTlsClient;
LTEScanner LteScanner;
LTEModemVerification LteModem;
/* MP */
#include <MP.h>
/* MQTT */
MqttClient mqttClient( LteClient );
MqttClient mqttTlsClient( LteTlsClient );
/* WiFi */
#include "spresense-wifi.h"
AoiSpresense::WiFi Wifi;
AoiSpresense::WiFiClient *WifiClient = 0;

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
    bool Ast::m_audioAttention = false;
    uint32_t Ast::m_audioPlayer0BufferSize = PLAYER_BUFFER_SIZE;
    uint32_t Ast::m_audioPlayer1BufferSize = PLAYER_BUFFER_SIZE;
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
            { "attachInterrupt", &Arduino::attachInterrupt },
            { "delay", &Arduino::delay },
            { "delayMicroseconds", &Arduino::delayMicroseconds },
            { "detachInterrupt", &Arduino::detachInterrupt },
            { "digitalRead", &Arduino::digitalRead },
            { "digitalWrite", &Arduino::digitalWrite },
            { "led", &Ast::led },
            { "micros", &Arduino::micros },
            { "millis", &Arduino::millis },
            { "noTone", &Arduino::noTone },
            { "pinMode", &Ast::pinMode },
            { "tone", &Arduino::tone },
            { "yield", &Arduino::yield },
            /* Audio */
            { "audioBegin", &Ast::audioBegin },
            { "audioEnd", &Ast::audioEnd },
            { "audioInitPlayer", &Ast::audioInitPlayer },
            { "audioInitRecorder", &Ast::audioInitRecorder },
            { "audioPlay", &Ast::audioPlay },
            { "audioRecord", &Ast::audioRecord },
            { "audioSetBeep", &Ast::audioSetBeep },
            { "audioSetPlayerMode", &Ast::audioSetPlayerMode },
            { "audioSetReadyMode", &Ast::audioSetReadyMode },
            { "audioSetRecorderMode", &Ast::audioSetRecorderMode },
            { "audioSetVolume", &Ast::audioSetVolume },
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
            { "gnssSave", &Ast::gnssSaveEphemeris },
            /* LowPower */
            { "clockMode", &Ast::clockMode },
            { "coldSleep", &Ast::coldSleep },
            { "current", &Ast::current },
            { "deepSleep", &Ast::deepSleep },
            { "dmesg", &Ast::dmesg },
            { "reboot", &Ast::reboot },
            { "sleep", &Ast::sleep },
            { "voltage", &Ast::voltage },
            /* HTTP */
            { "httpBegin", &Ast::httpBegin },
            { "httpGet", &AoiUtil::Http::httpGet },
            { "httpGetRaw", &Ast::httpGetRaw },
            { "httpGetStream", &Ast::httpGetStream },
            { "httpPost", &Ast::httpPost },
            { "httpPostStream", &Ast::httpPostStream },
            /* LTE */
            { "lteBegin", &Ast::lteBegin },
            { "lteConfig", &Ast::lteConfig },
            { "lteEnd", &Ast::lteEnd },
            { "lteRtc", &Ast::lteRtc },
            /* MQTT */
            { "mqttBegin", &Ast::mqttBegin },
            { "mqttConnect", &AoiUtil::Mqtt::mqttConnect },
            { "mqttPoll", &AoiUtil::Mqtt::mqttPoll },
            { "mqttPublish", &AoiUtil::Mqtt::mqttPublish },
            { "mqttSubscribe", &AoiUtil::Mqtt::mqttSubscribe },
            /* RTC */
            { "date", &Ast::date },
            /* Watchdog */
            { "watchdogBegin", &Ast::watchdogBegin },
            { "watchdogKick", &Ast::watchdogKick },
            { "watchdogEnd", &Ast::watchdogEnd },
            { "watchdogTimeleft", &Ast::watchdogTimeleft },
            /* WiFi */
            { "wifiBegin", &Ast::wifiBegin },
            { "wifiConfig", &Ast::wifiConfig },
            { "wifiEnd", &Ast::wifiEnd },
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
        /* LTE */
        http = &LteClient;
        /* Watchdog */
        Watchdog.begin();
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
     * @fn String Ast::pinMode( StringList *args )
     *
     * @see String Arduino::pinMode( StringList *args )
     */
    String Ast::pinMode( StringList *args )
    {
        String s;
        String t;
        uint8_t i = 0;

        switch( count(args) )
        {
            case 2:
                i = _atoi( 0 );
                t = _a( 1 );
                if( t=="INPUT_PULLDOWN" )
                    ::pinMode( i, INPUT_PULLDOWN );
                else
                    s = Arduino::pinMode( args );
                break;
            default:
                s = usage( "pinMode pin (INPUT|OUTPUT|INPUT_PULLUP|INPUT_PULLDOWN)" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::audioBegin( StringList *args )
     *
     * Begin audio function.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::audioBegin( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                theAudio->begin( &Ast::audioAttentionCallback );
                break;
            default:
                s = usage( "audioBegin" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::audioInitPlayer( StringList *args )
     *
     * Initializes and sets Player action. 
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::audioInitPlayer( StringList *args )
    {
        String s;
        AudioClass::PlayerId id;
        int i = 24;
        char path[ i ];
        uint8_t codec;
        uint8_t channel;
        err_t r;

        switch( count(args) )
        {
            case 6:
                if( !playerIdFromString(_a(0),&id) ||
                    !codecTypeFromString(_a(1),&codec) ||
                    !channelFromString(_a(5),&channel) )
                    s = audioInitPlayer( 0 );
                else
                {
                    memset( path, 0, i );
                    _a( 2 ).toCharArray( path, i );

                    r = theAudio->initPlayer( id, codec, path, _atoi(3), _atoi(4), channel );
                    if( r!=AUDIOLIB_ECODE_OK )
                        s = audioInitPlayer( 0 );
                }
                break;
            default:
                s = usage( "initPlayer (ID0|ID1) (MP3|WAV) path sampling bit (MONO|STEREO)" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::audioInitRecorder( StringList *args )
     *
     * Initializes and sets Recorder action. 
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::audioInitRecorder( StringList *args )
    {
        String s;
        int i = 24;
        char path[ i ];
        uint8_t codec;
        uint8_t channel;
        uint32_t bitRate = AS_BITRATE_96000;
        err_t r;

        switch( count(args) )
        {
            case 6:
                bitRate = _atoi( 5 );
            case 5:
                if( !codecTypeFromString(_a(0),&codec) ||
                    !channelFromString(_a(4),&channel) )
                    s = audioInitRecorder( 0 );
                else
                {
                    memset( path, 0, i );
                    _a( 1 ).toCharArray( path, i );

                    r = theAudio->initRecorder( codec, path, _atoi(2), _atoi(3), channel, bitRate );
                    if( r!=AUDIOLIB_ECODE_OK )
                        s = audioInitRecorder( 0 );
                }
                break;
            default:
                s = usage( "initRecorder (MP3|WAV) path sampling bit (MONO|STEREO) bitRate?" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::audioPlay( StringList *args )
     *
     * Play audio. 
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::audioPlay( StringList *args )
    {
        String s;
        AudioClass::PlayerId id;
        err_t r;
        File f;

        m_audioAttention = false;

        switch( count(args) )
        {
            case 2:
                if( !playerIdFromString(_a(0),&id) ||
                    !AstStorage->exists(_a(1)) )
                    s = audioPlay( 0 );
                else
                {
                    f = AstStorage->open( _a(1) );
                    // send first frames to be decoded 
                    r = theAudio->writeFrames( id, f );
                    if( f && ((r==AUDIOLIB_ECODE_OK) || (r==AUDIOLIB_ECODE_FILEEND)) )
                    {
                        theAudio->startPlayer( id );
                        while( !m_audioAttention )
                        {
                            r = theAudio->writeFrames( id, f );
                            if( r!=AUDIOLIB_ECODE_OK )
                                break;
                            // adjusted by the time to read the audio stream file
                            usleep( 40000 );
                        }
                        theAudio->stopPlayer( id );
                        f.close();
                    }
                }
                break;
            default:
                s = usage( "audioPlay (ID0|ID1) file" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::audioRecord( StringList *args )
     *
     * Record audio. 
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::audioRecord( StringList *args )
    {
        String s;
        err_t r;
        File f;
        unsigned long msec = ::millis();

        m_audioAttention = false;

        switch( count(args) )
        {
            case 1:
                // interrupt
                if( AstStorage->exists(_a(0)) )
                    AstStorage->remove( _a(0) );
                f = AstStorage->open( _a(0), FILE_WRITE );
                if( f )
                {
                    theAudio->startRecorder();
                    while( !m_audioAttention && !Arduino::isInterrupted() )
                    {
                        r = theAudio->readFrames( f );
                        if( r!=AUDIOLIB_ECODE_OK )
                            break;
                        // adjusted by the time to write the audio stream file
                        usleep( 10000 );
                    }
                    theAudio->stopRecorder();
                    theAudio->closeOutputFile( f );
                    f.close();
                }
                break;
            case 2:
                // timeout
                if( AstStorage->exists(_a(0)) )
                    AstStorage->remove( _a(0) );
                f = AstStorage->open( _a(0), FILE_WRITE );
                if( f )
                {
                    theAudio->startRecorder();
                    while( !m_audioAttention && ((::millis()-msec)<_atoi(1)) )
                    {
                        r = theAudio->readFrames( f );
                        if( r!=AUDIOLIB_ECODE_OK )
                            break;
                        // adjusted by the time to write the audio stream file
                        usleep( 10000 );
                    }
                    theAudio->stopRecorder();
                    theAudio->closeOutputFile( f );
                    f.close();
                }
                break;
            default:
                s = usage( "audioRecord file (duration)?" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::audioEnd( StringList *args )
     *
     * End audio function.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::audioEnd( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                theAudio->setReadyMode();
                theAudio->end();
                break;
            default:
                s = usage( "audioEnd" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::audioSetBeep( StringList *args )
     *
     * Set Beep Sound.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::audioSetBeep( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 3:
                theAudio->setBeep( _atoi(0), _atoi(1), _atoi(2) );
                break;
            default:
                s = usage( "audioSetBeep (0|1) -90~0 frequency" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::audioSetPlayerMode( StringList *args )
     *
     * Set Audio Library Mode to Music Player.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::audioSetPlayerMode( StringList *args )
    {
        String s;
        AsSetPlayerOutputDevice device;
        AsSpDrvMode mode;
        uint32_t buffer0 = m_audioPlayer0BufferSize;
        uint32_t buffer1 = m_audioPlayer1BufferSize;

        switch( count(args) )
        {
            case 4:
                buffer1 = _atoi( 3 );
                m_audioPlayer1BufferSize = buffer1;
            case 3:
                buffer0 = _atoi( 2 );
                m_audioPlayer0BufferSize = buffer0;
            case 2:
                if( !playerOutputDeviceFromString(_a(0),&device) ||
                    !playerSpeakerDriverModeFromString(_a(1),&mode) )
                    s = audioSetPlayerMode( 0 );
                else
                    theAudio->setPlayerMode( device, mode, buffer0, buffer1 );
                break;
            case 0:
                theAudio->setPlayerMode( AS_SETPLAYER_OUTPUTDEVICE_SPHP, 0, 0 );
                break;
            default:
                s = usage( "audioSetPlayerMode (SPHP|I2C) (LINEOUT|1DRIVER|2DRIVER|4DRIVER) (buffer0)? (buffer1)?" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::audioSetReadyMode( StringList *args )
     *
     * Set Audio Library Mode to Ready.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::audioSetReadyMode( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                // refresh
                m_audioAttention = false;
                m_audioPlayer0BufferSize = PLAYER_BUFFER_SIZE;
                m_audioPlayer1BufferSize = PLAYER_BUFFER_SIZE;

                theAudio->setReadyMode();
                break;
            default:
                s = usage( "audioSetReadyMode" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::audioSetRecorderMode( StringList *args )
     *
     * Set Audio Library Mode to Sound Recorder.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::audioSetRecorderMode( StringList *args )
    {
        String s;
        AsSetRecorderStsInputDevice device;
        int32_t gain = 200;
        uint32_t buffer = 8192;

        switch( count(args) )
        {
            case 3:
                buffer = _atoi( 2 );
            case 2:
                gain = _atoui( 1 );
            case 1:
                if( !recorderInputDeviceFromString(_a(0),&device) )
                    s = audioSetRecorderMode( 0 );
                else
                    theAudio->setRecorderMode( device, gain, buffer );
                break;
            default:
                s = usage( "audioSetRecorderMode (MIC|I2S) (0-210) (buffer)?" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::audioSetVolume( StringList *args )
     *
     * Set sound volume.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::audioSetVolume( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
                theAudio->setVolume( _atoi(0) );
                break;
            default:
                s = usage( "audioSetVolume -1020~120" );
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
/*
            // Auto
                r = theCamera.setAutoExposure( true );
                if( r!=CAM_ERR_SUCCESS )
                    return cameraBegin( 0 );
                r = theCamera.setAutoISOSensitivity( true );
                if( r!=CAM_ERR_SUCCESS )
                    return cameraBegin( 0 );
                r = theCamera.setAutoWhiteBalance( true );
                if( r!=CAM_ERR_SUCCESS )
                    return cameraBegin( 0 );
*/
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
                if( !AstStorage->exists(root) )
                    return ll( 0 );
                d = AstStorage->open( root );
                p = String(d.name()) + (root.length()?"/":"");
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
                if( Flash.exists("gnss_backup.bin") )
                    r = Gnss.start( HOT_START );
                else
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
        char buf[ 33 ];
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
                        r[ "maps" ] = "https://www.google.com/maps?q="
                                     + String(nav.latitude,5) + "," + String(nav.longitude,5);
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
     * @fn String Ast::gnssSaveEphemeris( StringList *args )
     *
     * Save the data stored in the backup RAM to Flash
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::gnssSaveEphemeris( StringList *args )
    {
        String s;
        int r;

        switch( count(args) )
        {
            case 0:
                r = Gnss.saveEphemeris();
                if( r )
                    s = gnssSaveEphemeris( 0 );
                break;
            default:
                s = usage( "gnssSaveEphemeris" );
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
            case 0:
                s = prettyPrintTo( "value", LowPower.getClockMode() );
                break;
            case 1:
                LowPower.clockMode( static_cast<clockmode_e>(_atoi(0)) );
                board_xtal_power_control( static_cast<clockmode_e>(_atoi(0))!=CLOCK_MODE_8MHz );
                break;
            default:
                s = usage( "clockMode [0-2]*" );
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
     * @fn String Ast::current( StringList *args )
     *
     * Get the sensed battery current on CXD5247.
     *
     * @param[in] args Reference to arguments.
     * @return Sensed battery current.
     */
    String Ast::current( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                s = prettyPrintTo( "value", LowPower.getCurrent() );
                break;
            default:
                s = usage( "current" );
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
                r[ "resetReason" ] = resetReason( LowPower.bootCause() );
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
                if( (_atoi(0)<-1) || (40000<_atoi(0)) )
                    return reboot( 0 );

                if( _atoi(0)==-1 )
                    Watchdog.end();
                else
                {
                    Watchdog.begin();
                    Watchdog.start( _atoi(0) );
                }
                break;
            default:
                s = usage( "reboot (-1|0-40000)*" );
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
     * @fn String Ast::voltage( StringList *args )
     *
     * Get the sensed battery voltage on CXD5247.
     *
     * @param[in] args Reference to arguments.
     * @return Sensed battery voltage.
     */
    String Ast::voltage( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                s = prettyPrintTo( "value", LowPower.getVoltage() );
                break;
            default:
                s = usage( "voltage" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::httpBegin( StringList *args )
     *
     * Initalize https certs.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::httpBegin( StringList *args )
    {
        String s;
        File rc, cc, pk;

        switch( count(args) )
        {
            case 0:
                if( WifiClient )
                    http = WifiClient;
                else
                    http = &LteClient;
                break;
/*
 * Feature removed due to underlying issue.
 *
            case 3:
                http = &LteTlsClient;
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
                s = usage( "httpBegin CACert Certificate PrivateKey" );
*/
            default:
                s = usage( "httpBegin" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::httpGetRaw( StringList *args )
     *
     * Send HTTP GET to server and save raw data.
     *
     * @param[in] args Reference to arguments.
     * @return Recieved size.
     */
    String Ast::httpGetRaw( StringList *args )
    {
        String s;
        String t;
        String host;
        int port = 80;
        int timeout = 30 * 1000;
        // response
        File f;
        int start = ::millis();
        int size = 0;
        int i = 0;
        int j = 0;

        // for SPI connection
        if( WifiClient )
            WifiClient->setRawMode( true );

        char *buf = new char[ _AOIUTIL_HTTP_BUFFER_SIZE_+1 ];
        switch( count(args) )
        {
            case 5:
                timeout = _atoi( 4 ) * 1000;
            case 4:
                port = _atoi( 3 );
            case 3:
                host = _a( 0 );
                if( !http->connect(host.c_str(),port) )
                    return httpGetRaw( 0 );
            // GET
                http->println( "GET "+_a(1)+" HTTP/1.0" );
                http->println( "Host: " + host );
                http->println( "User-Agent: " + String(STR_USER_AGENT) );
                http->println( "Connection: " + String("close") );
                http->println();
            // Response
                t = _a( 2 ); 
                if( AstStorage->exists(t) )
                    AstStorage->remove( t );
                f = AstStorage->open( t, FILE_WRITE );
                while( true )
                {
                    if( i=http->available() )
                    {
                        i = http->read( (uint8_t*)buf, _AOIUTIL_HTTP_BUFFER_SIZE_ );
                        if( WifiClient && (i<=0) )
                            continue;
                        j = 0;
                        if( !size )
                            j = responseRaw( buf, i );
                        f.write( (uint8_t*)(buf+j), i-j );
                        size += (i-j);
                    }
                    if( !http->available() && !http->connected() )
                    {
                        http->stop();
                        break;
                    }
                // timeout
                    if( timeout<(::millis()-start) )
                        break;
                }
                f.close();
                s = prettyPrintTo( "value", size );
                break;
            default:
                s = usage( "httpGetRaw host path file (port timeout)?" );
                break;
        }
        delete [] buf;

        // for SPI connection
        if( WifiClient )
            WifiClient->setRawMode( false );

        return s;
    }
    /**
     * @fn String Ast::httpGetStream( StringList *args )
     *
     * Send HTTP GET to server and play stream.
     *
     * @param[in] args Reference to arguments.
     * @return Recieved size.
     */
    String Ast::httpGetStream( StringList *args )
    {
        String s;
        String t;
        String host;
        int port = 80;
        int timeout = 30 * 1000;
        // response
        File f;
        int start = ::millis();
        int size = 0;
        int i = 0;
        int j = 0;
        int k = 0;
        // stream
        AudioClass::PlayerId id = AudioClass::Player0;
        err_t r;
        bool bufferring = true;
        int bufferringSize = m_audioPlayer0BufferSize - _AOIUTIL_HTTP_BUFFER_SIZE_;
        int adjust = 10 * 2;

        m_audioAttention = false;

        // for SPI connection
        if( WifiClient )
            WifiClient->setRawMode( true );

        char *buf = new char[ _AOIUTIL_HTTP_BUFFER_SIZE_+1 ];
        switch( count(args) )
        {
            case 6:
                adjust = _atoi( 5 );
            case 5:
                timeout = _atoi( 4 ) * 1000;
            case 4:
                port = _atoi( 3 );
            case 3:
                host = _a( 0 );
                if( !http->connect(host.c_str(),port) )
                    return httpGetStream( 0 );
            // GET
                http->println( "GET "+_a(1)+" HTTP/1.0" );
                http->println( "Host: " + host );
                http->println( "User-Agent: " + String(STR_USER_AGENT) );
                http->println( "Connection: " + String("close") );
                http->println();
            // Response
                if( !playerIdFromString(_a(2),&id) )
                    return httpGetStream( 0 );
                if( id==AudioClass::Player1 )
                    bufferringSize = m_audioPlayer1BufferSize - _AOIUTIL_HTTP_BUFFER_SIZE_;
                while( true )
                {
                    if( i=http->available() )
                    {
                        i = http->read( (uint8_t*)buf, _AOIUTIL_HTTP_BUFFER_SIZE_ );                        
                        if( WifiClient && (i<=0) )
                            continue;
                        j = 0;
                        if( !size )
                            j = responseRaw( buf, i );
                        r = theAudio->writeFrames( id, (uint8_t*)(buf+j), i-j );
                        size += (i-j);

                        if( bufferring )
                        {
                            if( (bufferringSize<=size) && (r==AUDIOLIB_ECODE_OK) )
                            {
                                theAudio->startPlayer( id );
                                bufferring = false;
                            }
                        }
                        else if( !m_audioAttention && (r==AUDIOLIB_ECODE_SIMPLEFIFO_ERROR) )
                        {
                            for( k=0; k<adjust; k++ )
                            {
                                // adjusted by the time to read the audio stream file
                                usleep( 40000 );
                                r = theAudio->writeFrames( id, (uint8_t*)(buf+j), i-j );
                                if( r!=AUDIOLIB_ECODE_SIMPLEFIFO_ERROR )
                                    break;
                            }
                        }
                    }
                    if( !http->available() && !http->connected() )
                    {
                        http->stop();
                        break;
                    }
                // timeout
                    if( timeout<(::millis()-start) )
                        break;
                }
                theAudio->stopPlayer( id );
                s = prettyPrintTo( "value", size );
                break;
            default:
                s = usage( "httpGetStream host path (ID0|ID1) (port timeout adjust)?" );
                break;
        }
        delete [] buf;

        // for SPI connection
        if( WifiClient )
            WifiClient->setRawMode( false );

        return s;
    }
    /**
     * @fn String Ast::httpPost( StringList *args )
     *
     * Send HTTP POST to server.
     *
     * @param[in] args Reference to arguments.
     * @return Recieved content.
     */
    String Ast::httpPost( StringList *args )
    {
        String s, t, header, footer;
        File f;
        int size = 0;
        String host;
        int port = 80;
        int timeout = 30 * 1000;
        uint8_t *buf = 0;
        int bufSize = _AOIUTIL_HTTP_BUFFER_SIZE_;

        switch( count(args) )
        {
            case 5:
                timeout = _atoi( 4 ) * 1000;
            case 4:
                port = _atoi( 3 );
            case 3:
                t = _a( 2 );
                if( !AstStorage->exists(t) )
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
                http->print( "Content-Type: "+String("multipart/form-data")+"; " );
                http->println( "boundary=\""+String(STR_BOUNDARY)+"\"" );
                http->println( "Content-Length: "+String(size) );
                http->println( "Connection: " + String("close") );
                http->println();
                http->print( header );
              // for SPI connection
                if( WifiClient )
                    bufSize = 1500 - 7; // SPI_MAX_SIZE - HEADERSIZE in GS2200AtCmd.cpp
              // Upload file
                f = AstStorage->open( t, FILE_READ );
                buf = new uint8_t[ bufSize ];
                while( f.available() )
                {
                    size = f.read( buf, bufSize );
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
     * @fn String Ast::httpPostStream( StringList *args )
     *
     * Send audio stream to server using HTTP POST (chunked).
     *
     * @param[in] args Reference to arguments.
     * @return Recieved content.
     */
    String Ast::httpPostStream( StringList *args )
    {
        String s;
        err_t r;
        uint32_t size = 0;
        String host;
        int port = 80;
        int timeout = 30 * 1000;
        uint8_t *buf = 0;
        uint32_t bufSize = _AOIUTIL_HTTP_BUFFER_SIZE_;

        m_audioAttention = false;

        switch( count(args) )
        {
            case 4:
                timeout = _atoi( 3 ) * 1000;
            case 3:
                port = _atoi( 2 );
            case 2:
            // POST
                host = _a( 0 );
                if( !http->connect(host.c_str(),port) )
                    return httpPost( 0 );
                http->println( "POST " + _a(1) + " HTTP/1.1" );
                http->println( "Host: " + host );
                http->println( "User-Agent: " + String(STR_USER_AGENT) );
                http->println( "Content-Type: " + String("application/octet-stream") );
                http->println( "Transfer-Encoding: " + String("chunked") );
                http->println( "Connection: " + String("close") );
                http->println();
              // for SPI connection
                if( WifiClient )
                    bufSize = 1500 - 7; // SPI_MAX_SIZE - HEADERSIZE in GS2200AtCmd.cpp
              // Stream audio
                buf = new uint8_t[ bufSize ];
                theAudio->startRecorder();
                while( !m_audioAttention && !Arduino::isInterrupted() )
                {
                    r = theAudio->readFrames( buf, bufSize, &size );
                    if( (r!=AUDIOLIB_ECODE_OK) && (r!=AUDIOLIB_ECODE_INSUFFICIENT_BUFFER_AREA) )
                        break;
                    else if( !size )
                        continue;
                    http->println( String(size,HEX) );
                    http->write( buf, size );
                    http->println();
                }
                theAudio->stopRecorder();
                delete [] buf;
                http->println( String("0") );
                http->println();
            // Response
                s = response( timeout );
                s = prettyPrintTo( "value", s );
                break;
            default:
                s = usage( "httpPostStream host path (port timeout)?" );
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
                        s = STR_CANT_CONNECT_TO_WIRELESS_NETWORK;
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
     * @fn String Ast::lteRtc( StringList *args )
     *
     * Get rtc from the LTE network.
     *
     * @param[in] args Reference to arguments.
     * @return unixtime string.
     */
    String Ast::lteRtc( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                if( Lte.getStatus()!=LTE_READY )
                    s = lteRtc( 0 );
                else
                {
                    s = String( Lte.getTime() );
                    s = prettyPrintTo( "value", s );
                }
                break;
            default:
                s = usage( "lteRtc" );
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
            case 0:
                mqtt = &mqttClient;
                break;
            case 3:
                mqtt = &mqttTlsClient;
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
     * @fn String Ast::date( StringList *args )
     *
     * Print date.
     *
     * @param[in] args Reference to arguments.
     * @return date string.
     */
    String Ast::date( StringList *args )
    {
        String s;
        uint8_t size = 20;
        char *buf = NULL;
        RtcTime rtc;

        switch( count(args) )
        {
            case 0:
                rtc = RTC.getTime();
                buf = new char[ size ];
                snprintf( buf, size,
                          "%04d-%02d-%02dT%02d:%02d:%02d",
                          rtc.year(), rtc.month(), rtc.day(),
                          rtc.hour(), rtc.minute(), rtc.second() );
                s = prettyPrintTo( "value", buf );
                delete [] buf;
                break;
            case 1:
                rtc.unixtime( _atoul(0) );
                RTC.setTime( rtc );
                break;
            default:
                s = usage( "date (unixtime)?" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::watchdogBegin( StringList *args )
     *
     * Initialize the Watchdog and start to check timer(mesc).
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::watchdogBegin( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
                Watchdog.start( _atoi(0) );
                break;
            default:
                s = usage( "watchdogBegin 1-40000" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::watchdogEnd( StringList *args )
     *
     * Stop to check timer for avoid bite watchdog.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::watchdogEnd( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                Watchdog.stop();
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
    String Ast::watchdogKick( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                Watchdog.kick();
                break;
            default:
                s = usage( "watchdogKick" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::watchdogTimeleft( StringList *args )
     *
     * Get a remain time for bite watchdog.
     *
     * @param[in] args Reference to arguments.
     * @return Remain time to expire timeout(mesc).
     */
    String Ast::watchdogTimeleft( StringList *args )
    {
        String s;
        uint32_t i = 0;

        switch( count(args) )
        {
            case 0:
                i = Watchdog.timeleft();
                s = prettyPrintTo( "value" , i );
                break;
            default:
                s = usage( "watchdogTimeleft" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::wifiBegin( StringList *args )
     *
     * Connect to wireless network.
     *
     * @param[in] args Reference to arguments.
     * @return Wireless ip address, Otherwise error string.
     */
    String Ast::wifiBegin( StringList *args )
    {
        String s;

        if( WifiClient )
            delete WifiClient;
        WifiClient = 0;

        switch( count(args) )
        {
            case 2:
                if( !Wifi.begin() )
                    s = wifiBegin( 0 );
                else
                {
                    if( !Wifi.attach(_a(0),_a(1)) )
                        s = STR_CANT_CONNECT_TO_WIRELESS_NETWORK;
                    else
                    {
                        WifiClient = new AoiSpresense::WiFiClient();

                        StringList sl;
                        s = wifiConfig( &sl );
                    }
                }
                break;
            default:
                s = usage( "wifiBegin ssid password" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::wifiConfig( StringList *args )
     *
     * Return WiFi network information.
     *
     * @param[in] args Reference to arguments.
     * @return Network information.
     */
    String Ast::wifiConfig( StringList *args )
    {
        String s;
        NetworkStatus ns;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        switch( count(args) )
        {
            case 0:
                ns = Wifi.networkStatus();
                r[ "ipAddress" ] = ns.localIP;
                r[ "subnetMask" ] = ns.subnetMask;
                r[ "gatewayIp" ] = ns.gatewayIP;
                r[ "macAddress" ] = ns.macAddress;
                r[ "dnsIP1" ] = ns.dnsIP1;
                r[ "dnsIP2" ] = ns.dnsIP2;
                r.prettyPrintTo( s );
                break;
            default:
                s = usage( "wifiConfig" );
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::wifiEnd( StringList *args )
     *
     * Detach from wireless network.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Ast::wifiEnd( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                if( WifiClient )
                    delete WifiClient;
                WifiClient = 0;
                Wifi.end();
                break;
            default:
                s = usage( "wifiEnd" );
                break;
        }

        return s;
    }
    /**
     * @fn void Ast::audioAttentionCallback( const ErrorAttentionParam *param )
     *
     * This method is call when audio internal error occurs.
     *
     * @param[in] param ErrorAttentionParam
     */
    void Ast::audioAttentionCallback( const ErrorAttentionParam *param )
    {
        m_audioAttention = (AS_ATTENTION_CODE_WARNING<=param->error_code);
    }
    /**
     * @fn bool Ast::channelFromString( const String &value, uint8_t *channel )
     *
     * Return channel from string.
     *
     * @param[in] value channel string like "MONO".
     * @param[in,out] type reference to channel.
     * @return Return true if value is valid, Otherwise return false.
     */
    bool Ast::channelFromString( const String &value, uint8_t *channel )
    {
        bool b = true;

        if( value=="MONO" )
            *channel = AS_CHANNEL_MONO;
        else if( value=="STEREO" )
            *channel = AS_CHANNEL_STEREO;
        else
            *channel = AoiCore::toInt( value );

        return b;
    }
    /**
     * @fn bool Ast::codecTypeFromString( const String &value, uint8_t *type )
     *
     * Return codec type from string.
     *
     * @param[in] value codec type string like "MP3".
     * @param[in,out] type reference to codec type.
     * @return Return true if value is valid, Otherwise return false.
     */
    bool Ast::codecTypeFromString( const String &value, uint8_t *type )
    {
        bool b = true;

        if( value=="MP3" )
            *type = AS_CODECTYPE_MP3;
        else if( value=="WAV" )
            *type = AS_CODECTYPE_WAV;
        else
            *type = AoiCore::toInt( value );

        return b;
    }
    /**
     * @fn bool Ast::playerIdFromString( const String &value, AudioClass::PlayerId *id )
     *
     * Return PlayerId from string.
     *
     * @param[in] value output device type string like "ID0".
     * @param[in,out] id reference to PlayerId.
     * @return Return true if value is valid, Otherwise return false.
     */
    bool Ast::playerIdFromString( const String &value, AudioClass::PlayerId *id )
    {
        bool b = true;

        if( value=="ID0" )
            *id = AudioClass::Player0;
        else if( value=="ID1" )
            *id = AudioClass::Player1;
        else
            b = false;

        return b;
    }
    /**
     * @fn bool Ast::playerOutputDeviceFromString( const String &value, AsSetPlayerOutputDevice *device )
     *
     * Return AsSetPlayerOutputDevice from string.
     *
     * @param[in] value output device type string like "SPHP".
     * @param[in,out] device reference to AsSetPlayerOutputDevice.
     * @return Return true if value is valid, Otherwise return false.
     */
    bool Ast::playerOutputDeviceFromString( const String &value, AsSetPlayerOutputDevice *device )
    {
        bool b = true;

        if( value=="SPHP" )
            *device = AS_SETPLAYER_OUTPUTDEVICE_SPHP;
        else if( value=="I2S" )
            *device = AS_SETPLAYER_OUTPUTDEVICE_I2SOUTPUT;
        else
            b = false;

        return b;
    }
    /**
     * @fn bool Ast::playerSpeakerDriverModeFromString( const String &value, AsSpDrvMode *mode )
     *
     * Return AsSpDrvMode from string.
     *
     * @param[in] value output device type string like "LINEOUT".
     * @param[in,out] device reference to AsSpDrvMode.
     * @return Return true if value is valid, Otherwise return false.
     */
    bool Ast::playerSpeakerDriverModeFromString( const String &value, AsSpDrvMode *mode )
    {
        bool b = true;

        if( value=="LINEOUT" )
            *mode = AS_SP_DRV_MODE_LINEOUT;
        else if( value=="1DRIVER" )
            *mode = AS_SP_DRV_MODE_1DRIVER;
        else if( value=="2DRIVER" )
            *mode = AS_SP_DRV_MODE_2DRIVER;
        else if( value=="4DRIVER" )
            *mode = AS_SP_DRV_MODE_4DRIVER;
        else
            b = false;

        return b;
    }
    /**
     * @fn bool Ast::recorderInputDeviceFromString( const String &value, AsSetRecorderStsInputDevice *device )
     *
     * Return AsSetRecorderStsInputDevice from string.
     *
     * @param[in] value input device type string like "MIC".
     * @param[in,out] device reference to AsSetRecorderStsInputDevice.
     * @return Return true if value is valid, Otherwise return false.
     */
    bool Ast::recorderInputDeviceFromString( const String &value, AsSetRecorderStsInputDevice *device )
    {
        bool b = true;

        if( value=="MIC" )
            *device = AS_SETRECDR_STS_INPUTDEVICE_MIC;
        else if( value=="I2S" )
            *device = AS_SETRECDR_STS_INPUTDEVICE_I2S;
        else
            b = false;

        return b;
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
     * @fn String Ast::resetReason( bootcause_e bootcause )
     *
     * Returns reset reason string.
     *
     * @return Reset reason string.
     * @see https://developer.sony.com/spresense/spresense-api-references-arduino/group__lowpower.html
     */
    String Ast::resetReason( bootcause_e bootcause )
    {
        String s;

        switch( bootcause )
        {
            case 0 :
                s = "Power On Reset with Power Supplied";
                break;
            case 1 :
                s = "System WDT expired or Self Reboot";
                break;
            case 2 :
                s = "Chip WDT expired";
                break;
            case 3 :
                s = "In DeepSleep state, Detected WKUPL signal";
                break;
            case 4 :
                s = "In DeepSleep state, Detected WKUPS signal";
                break;
            case 5 :
                s = "In DeepSleep state, RTC Alarm expired";
                break;
            case 6 :
                s = "In DeepSleep state, USB Connected";
                break;
            case 7 :
                s = "In DeepSleep state, Reserved others cause occurred";
                break;
            case 8 :
                s = "In ColdSleep state, Detected SCU Interrupt";
                break;
            case 9 :
                s = "In ColdSleep state, RTC Alarm0 expired";
                break;
            case 10 :
                s = "In ColdSleep state, RTC Alarm1 expired";
                break;
            case 11 :
                s = "In ColdSleep state, RTC Alarm2 expired";
                break;
            case 12 :
                s = "In ColdSleep state, RTC Alarm Error occurred";
                break;
            case 16 :
                s = "In ColdSleep state, Detected GPIO IRQ 36";
                break;
            case 17 :
                s = "In ColdSleep state, Detected GPIO IRQ 37";
                break;
            case 18 :
                s = "In ColdSleep state, Detected GPIO IRQ 38";
                break;
            case 19 :
                s = "In ColdSleep state, Detected GPIO IRQ 39";
                break;
            case 20 :
                s = "In ColdSleep state, Detected GPIO IRQ 40";
                break;
            case 21 :
                s = "In ColdSleep state, Detected GPIO IRQ 41";
                break;
            case 22 :
                s = "In ColdSleep state, Detected GPIO IRQ 42";
                break;
            case 23 :
                s = "In ColdSleep state, Detected GPIO IRQ 43";
                break;
            case 24 :
                s = "In ColdSleep state, Detected GPIO IRQ 44";
                break;
            case 25 :
                s = "In ColdSleep state, Detected GPIO IRQ 45";
                break;
            case 26 :
                s = "In ColdSleep state, Detected GPIO IRQ 46";
                break;
            case 27 :
                s = "In ColdSleep state, Detected GPIO IRQ 47";
                break;
            case 28 :
                s = "In ColdSleep state, Detected SEN_INT Interrupt";
                break;
            case 29 :
                s = "In ColdSleep state, Detected PMIC Interrupt";
                break;
            case 30 :
                s = "In ColdSleep state, USB Disconnected";
                break;
            case 31 :
                s = "In ColdSleep state, USB Connected";
                break;
            case 32 :
                s = "Power On Reset";
                break;
            default :
                s = "No mean";
                break;
        }

        return s;
    }
    /**
     * @fn String Ast::requestBodyHeaderInPut( const String &value )
     *
     * Return request body header in HTTP PUT.
     *
     * @param[in] boundary Boundary string.
     * @param[in] name Content-Disposition: name attribute string.
     * @param[in] value Putted value.
     * @param[in/out] size Putted value size. If value is file, File size is used.
     * @return Request body header string in HTTP PUT.
     */
    String Ast::requestBodyHeaderInPut( const String &boundary, const String &name, const String &value, int *size )
    {
        String s;

    // If file is exitst, use file size
        if( !AstStorage->exists(value) )
            s = AoiUtil::Http::requestBodyHeaderInPut( boundary, name, value,size );
        else
        {
            s += "--" + boundary + "\r\n";
            s += "Content-Disposition: form-data; name=\"" + name + "\";";
            s += " filename=\"" + value + "\"\r\n";
            s += "Content-Type: application/octet-stream\r\n";
            s += "Content-Transfer-Encoding: binary\r\n";
            File f = AstStorage->open( value, FILE_READ );
            *size = f.size();
            f.close();
            s += "\r\n";
        }

        return s;
    }
}
#endif
