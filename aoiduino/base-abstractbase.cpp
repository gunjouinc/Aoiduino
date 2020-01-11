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
#include "base-shell.h"
#include "product-template.h"
#include "base-abstractbase.h"

/**
 * @namespace AoiBase
 * @brief Aoi base classes.
 */
namespace AoiBase
{
// Static variables.
    AbstractSerial *AbstractBase::m_abstractSerial = 0;
    AoiProduct::Template *AbstractBase::m_template = 0;
    InterfaceType AbstractBase::m_interfaceType = ::_Serial;
    /**
     * @fn AbstractBase::AbstractBase( void )
     *
     * Constructor. Member variables are initialized.
     */
    AbstractBase::AbstractBase( void )
    {
        if( !m_abstractSerial )
            m_abstractSerial = new AbstractSerial;
    }
    /**
     * @fn AbstractBase::~AbstractBase( void )
     *
     * Destructor. Member variables are deleted.
     */
    AbstractBase::~AbstractBase( void )
    {
    }
    /**
     * @fn bool AbstractBase::begin( StringList *args, InterfaceType type = _Serial )
     *
     * Sets interface type and begin interface.
     *
     * @param[in] args Interface parameter.
     * @param[in] type Interface type.
     */
    bool AbstractBase::begin( StringList *args, InterfaceType type )
    {
        bool b = false;

        m_interfaceType = type;
    // Selects interface.
        switch( interfaceType() )
        {
            case ::_Serial:
                b = m_abstractSerial->begin( args );
                break;
            default:
                break;
        }

        return b;
    }
    /**
     * @fn int AbstractBase::bytesAvailable( void )
     *
     * @see AbstractCore::bytesAvailable( void ).
     */
    int AbstractBase::bytesAvailable( void )
    {
        int i = 0;
    // Selects interface.
        switch( interfaceType() )
        {
            case ::_Serial:
                i = m_abstractSerial->bytesAvailable();
                break;
            default:
                break;
        }

        return i;
    }
    /**
     * @fn void AbstractBase::end( void )
     *
     * @see AbstractCore::end( void ).
     */
    void AbstractBase::end( void )
    {
    // Selects interface.
        switch( interfaceType() )
        {
            case ::_Serial:
                m_abstractSerial->end();
                break;
            default:
                break;
        }
    }
    /**
     * @fn void AbstractBase::flush( void )
     *
     * @see AbstractCore::flush( void ).
     */
    void AbstractBase::flush( void )
    {
    // Selects interface.
        switch( interfaceType() )
        {
            case ::_Serial:
                m_abstractSerial->flush();
                break;
            default:
                break;
        }
    }
    /**
     * @fn InterfaceType AbstractBase::interfaceType( void ) const
     *
     * Returns interface types.
     *
     * @return Interface types.
     */
    InterfaceType AbstractBase::interfaceType( void ) const
    {
        return m_interfaceType;
    }
    /**
     * @fn bool AbstractBase::isBegan( void )
     *
     * @see AbstractCore::isBegan( void ).
     */
    bool AbstractBase::isBegan( void )
    {
        bool b = false;
    // Selects interface.
        switch( interfaceType() )
        {
            case ::_Serial:
                b = m_abstractSerial->isBegan();
                break;
            default:
                break;
        }

        return b;
    }
    /**
     * @fn String AbstractBase::read( void )
     *
     * @see AbstractCore::read( void ).
     */
    String AbstractBase::read( void )
    {
        String s;
    // Selects interface.
        switch( interfaceType() )
        {
            case ::_Serial:
                s = m_abstractSerial->read();
                break;
            default:
                break;
        }

        return s;
    }
    /**
     * @fn void AbstractBase::setTemplate( AoiProduct::Template *product )
     *
     * Sets template for internal loop.
     *
     * @param[in] product Template for internal loop.
     */
    void AbstractBase::setTemplate( AoiProduct::Template *product )
    {
        m_template = product;
    }
    /**
     * @fn int AbstractBase::write( const String &data )
     *
     * @see AbstractCore::write( const String &data ).
     */
    int AbstractBase::write( const String &data )
    {
        int i = 0;
    // Selects interface.
        switch( interfaceType() )
        {
            case ::_Serial:
                i = m_abstractSerial->write( data );
                break;
            default:
                break;
        }
        return i;
    }
    /**
     * @fn void AbstractBase::afterKeyInput( int input, String *buffer )
     *
     * Handles after key input.
     *
     * @param[in] input Input value.
     * @param[in] buffer Input buffer.
     */
    void AbstractBase::afterKeyInput( int input, String *buffer )
    {
        String s = String( char(input) );
        String *bf = buffer;
    // Writes to interface only serial. Because some network don't use echo mode.
        if( interfaceType()==::_Serial )
            app << s;

        switch( input )
        {
            case 0x08: // BS
                *bf = bf->substring( 0, bf->length()-1 );
                app << STR_SPACE;
                app << s;
                break;
            case 0x7f: // DEL
                *bf = bf->substring( 0, bf->length()-1 );
                app << KEY_BS;
                break;
            default:
                *bf += s;
                break;
        }
    }
    /**
     * @fn bool AbstractBase::beforeKeyInput( int input, String *buffer )
     *
     * Handles before key input.
     *
     * @param[in] input Input value.
     * @param[in] buffer Input buffer.
     * @return Returns true, If need after handling key input. Otherwise returns false.
     */
    bool AbstractBase::beforeKeyInput( int input, String *buffer )
    {
        bool b = false;
        String *bf = buffer;

        m_keyCode = (m_keyCode<<8) | input;
    // VT102 key code.
        switch( m_keyCode )
        {
            case 0x08:     // BS
            case 0x7f:     // DEL
                if( 0<bf->length() )
                    b = true;
                break;
            case 0x1b:     // Esc
                break;
            case 0x1b5b:   // Esc + [ - Erase buffer.
                for( int i=0; i<bf->length(); i++ )
                    app << KEY_BS;
                *bf = STR_NULL;
                break;
            case 0x1b5b41: // Up
            case 0x1b5b42: // Down
            case 0x1b5b43: // Right
            case 0x1b5b44: // Left
                break;
            default:
                b = true;
                break;
        }
    // Resets key code, if need.
        if( m_keyCode!=0x1b && m_keyCode!=0x1b5b )
            m_keyCode = 0;

        return b;
    }
    /**
     * @fn void AbstractBase::internalLoop( void )
     *
     * Internal loop. Please call this method in your loop function.
     */
    void AbstractBase::internalLoop( void )
    {
        yield();
    // Practices product template loop on same task.
        handleTemplate( this );
    }
    /**
     * @fn int AbstractBase::operator~( void )
     *
     * Flushes read and write buffer.
     */
    int AbstractBase::operator~( void )
    {
        int i = 1;

        while( bytesAvailable() )
            read();
        flush();

        return i;
    }
    /**
     * @fn int AbstractBase::operator!( void )
     *
     * Returns number of bytes available.
     *
     * @return Number of bytes available.
     */
    int AbstractBase::operator!( void )
    {
        return bytesAvailable();
    }
    /**
     * @fn int AbstractBase::operator>>( String *data )
     *
     * Reads first byte from interface. And appends data.
     *
     * @param[in] data Reference to string. If value was read, Appeds data to value.
     * @return Returns -1 if no data, Otherwise read value.
     */
    int AbstractBase::operator>>( String *data )
    {
        int i = -1;
    // Selects interface.
        switch( interfaceType() )
        {
            case ::_Serial:
                i = m_abstractSerial->readByte();
                break;
            default:
                break;
        }
        if( (i!=-1) && data )
            *data += String( i );

        return i;
    }
    /**
     * @fn int AbstractBase::operator<<( const String &data )
     *
     * Writes string data to interface.
     *
     * @param[in] data Data to write.
     * @return Writen byte count.
     */
    int AbstractBase::operator<<( const String &data )
    {
        return write( data );
    }
    /**
     * @fn int AbstractBase::operator<=( const String &data )
     *
     * Writes string data to interface with new line.
     *
     * @param[in] value Data to write.
     * @return Writen byte count.
     */
    int AbstractBase::operator<=( const String &data )
    {
        return write( _n+data );
    }
    /**
     * @fn void AbstractBase::handleTemplate( void *obj )
     *
     * Handles product template.
     *
     * @param[in] obj Reference to AbstractBase object.
     */
    void AbstractBase::handleTemplate( void *obj )
    {
        if( !m_template )
            return;
    // Practicies loop function.
        String s = m_template->loop();
        if( !s.length() )
            return;
    // Outputs message to app.
        AbstractBase *o = static_cast<AbstractBase*>( obj );
        if( o )
            *o << s;
    }
}
