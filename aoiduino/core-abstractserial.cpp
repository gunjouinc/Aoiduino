/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (aoiduino@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/masaomiura/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#include "core-abstractserial.h"
/** Serial interface. */
#define _i Serial
/**
 * @namespace AoiCore
 * @brief Aoi core classes.
 */
namespace AoiCore
{
    // Static variables.
    bool AbstractSerial::m_boolBegan = false;
    int AbstractSerial::m_intTimeout = 1;
    /**
     * @fn AbstractSerial::AbstractSerial( void )
     *
     * Constructor. Member variables are initialized.
     */
    AbstractSerial::AbstractSerial( void )
    {
    }
    /**
     * @fn AbstractSerial::~AbstractSerial( void )
     *
     * Destructor. Member variables are deleted.
     */
    AbstractSerial::~AbstractSerial( void )
    {
    }
    /**
     * @fn bool AbstractSerial::begin( StringList *args )
     *
     * @see AbstractCore::begin( StringList *args ).
     */
    bool AbstractSerial::begin( StringList *args )
    {
        m_boolBegan = false;

        switch( count(args) )
        {
            case 2:
#ifdef ESP8266
                _i.begin( _atol(0), static_cast<SerialConfig>(_atoi(1)) );
#else
                _i.begin( _atol(0), _atoi(1) );
#endif
                m_boolBegan = true;
                break;
            default:
                break;
        }

        return m_boolBegan;
    }
    /**
     * @fn int AbstractSerial::bytesAvailable( void )
     *
     * @see AbstractCore::bytesAvailable( void ).
     */
    int AbstractSerial::bytesAvailable( void )
    {
        return _i.available();
    }
    /**
     * @fn void AbstractSerial::end( void )
     *
     * @see AbstractCore::end( void ).
     */
    void AbstractSerial::end( void )
    {
        _i.end();

        m_boolBegan = false;
    }
    /**
     * @fn void AbstractSerial::flush( void )
     *
     * @see AbstractCore::flush( void ).
     */
    void AbstractSerial::flush( void )
    {
        _i.flush();
    }
    /**
     * @fn bool AbstractSerial::isBegan( void )
     *
     * @see AbstractCore::isBegan( void ).
     */
    bool AbstractSerial::isBegan( void )
    {
        return m_boolBegan;
    }
    /**
     * @fn String AbstractSerial::read( void )
     *
     * @see AbstractCore::read( void ).
     */
    String AbstractSerial::read( void )
    {
        int i = AbstractCore::bytesAvailable( timeout() );
    // Reads data.
        String s;
        while( i )
        {
            char c = _i.read();
            s += c;
        // If there is terminal character, Breaks while for faster response.
            if( String(c)==_term )
            {
                _i.flush();
                break;
            }
            i = AbstractCore::bytesAvailable( timeout() );
        }

        return s;
    }
    /**
     * @fn int AbstractSerial::readByte( void )
     *
     * @see AbstractCore::readByte( void ).
     */
    int AbstractSerial::readByte( void )
    {
        AbstractCore::bytesAvailable( timeout() );

        return _i.read();
    }
    /**
     * @fn void AbstractSerial::setTimeout( int msec = 1 )
     *
     * @see AbstractCore::setTimeout( int msec = 1 ).
     */
    void AbstractSerial::setTimeout( int msec )
    {
        m_intTimeout = msec;
    }
    /**
     * @fn int AbstractSerial::timeout( void ) const
     *
     * @see AbstractCore::timeout( void ).
     */
    int AbstractSerial::timeout( void ) const
    {
        return m_intTimeout;
    }/**
     * @fn int AbstractSerial::write( const String &data )
     *
     * @see AbstractCore::write( const String &data ).
     */
    int AbstractSerial::write( const String &data )
    {
        return _i.print( data );
    }
}
