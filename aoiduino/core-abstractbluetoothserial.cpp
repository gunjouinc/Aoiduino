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
#include "core-abstractbluetoothserial.h"
/** Serial interface. */
#include <BluetoothSerial.h>
BluetoothSerial BTSerial;
#define _i BTSerial
/**
 * @namespace AoiCore
 * @brief Aoi core classes.
 */
namespace AoiCore
{
    // Static variables.
    bool AbstractBluetoothSerial::m_boolBegan = false;
    int AbstractBluetoothSerial::m_intTimeout = 1;
    /**
     * @fn AbstractBluetoothSerial::AbstractBluetoothSerial( void )
     *
     * Constructor. Member variables are initialized.
     */
    AbstractBluetoothSerial::AbstractBluetoothSerial( void )
    {
    }
    /**
     * @fn AbstractBluetoothSerial::~AbstractBluetoothSerial( void )
     *
     * Destructor. Member variables are deleted.
     */
    AbstractBluetoothSerial::~AbstractBluetoothSerial( void )
    {
    }
    /**
     * @fn bool AbstractBluetoothSerial::begin( StringList *args )
     *
     * @see AbstractCore::begin( StringList *args ).
     */
    bool AbstractBluetoothSerial::begin( StringList *args )
    {
        m_boolBegan = false;

        switch( count(args) )
        {
            case 1:
                _i.begin( _a(0) );
                m_boolBegan = true;
                break;
            default:
                break;
        }

        return m_boolBegan;
    }
    /**
     * @fn int AbstractBluetoothSerial::bytesAvailable( void )
     *
     * @see AbstractCore::bytesAvailable( void ).
     */
    int AbstractBluetoothSerial::bytesAvailable( void )
    {
        return _i.available();
    }
    /**
     * @fn void AbstractBluetoothSerial::end( void )
     *
     * @see AbstractCore::end( void ).
     */
    void AbstractBluetoothSerial::end( void )
    {
        _i.end();

        m_boolBegan = false;
    }
    /**
     * @fn void AbstractBluetoothSerial::flush( void )
     *
     * @see AbstractCore::flush( void ).
     */
    void AbstractBluetoothSerial::flush( void )
    {
        _i.flush();
    }
    /**
     * @fn bool AbstractBluetoothSerial::isBegan( void )
     *
     * @see AbstractCore::isBegan( void ).
     */
    bool AbstractBluetoothSerial::isBegan( void )
    {
        return m_boolBegan;
    }
    /**
     * @fn String AbstractBluetoothSerial::read( void )
     *
     * @see AbstractCore::read( void ).
     */
    String AbstractBluetoothSerial::read( void )
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
     * @fn int AbstractBluetoothSerial::readByte( void )
     *
     * @see AbstractCore::readByte( void ).
     */
    int AbstractBluetoothSerial::readByte( void )
    {
        AbstractCore::bytesAvailable( timeout() );

        return _i.read();
    }
    /**
     * @fn void AbstractBluetoothSerial::setTimeout( int msec = 1 )
     *
     * @see AbstractCore::setTimeout( int msec = 1 ).
     */
    void AbstractBluetoothSerial::setTimeout( int msec )
    {
        m_intTimeout = msec;
    }
    /**
     * @fn int AbstractBluetoothSerial::timeout( void ) const
     *
     * @see AbstractCore::timeout( void ).
     */
    int AbstractBluetoothSerial::timeout( void ) const
    {
        return m_intTimeout;
    }/**
     * @fn int AbstractBluetoothSerial::write( const String &data )
     *
     * @see AbstractCore::write( const String &data ).
     */
    int AbstractBluetoothSerial::write( const String &data )
    {
        return _i.print( data );
    }
}
#endif
