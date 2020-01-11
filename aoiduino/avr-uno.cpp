/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (aoiduino@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/masaomiura/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#ifdef ARDUINO_AVR_UNO
#include "avr-uno.h"
/**
 * @namespace AoiAvr
 * @brief Aoi Avr classes.
 */
namespace AoiAvr
{
// Static variables.
    AoiBase::FunctionTable *Uno::m_functionTable = 0;
    /**
     * @fn AoiAvr::AoiAvr( void )
     *
     * Constructor. Member variables are initialized.
     */
    Uno::Uno( void )
    {
        if( m_functionTable )
            return;
    // Sets function table, If there is no instance.
        AoiBase::FunctionTable ftl[] =
        {
        // ^ Please set your function to use.
            { "analogRead", &Uno::analogRead },
            { "analogWrite", &Uno::analogWrite },
            { "delay", &Uno::delay },
            { "delayMicroseconds", &Uno::delayMicroseconds },
            { "digitalRead", &Uno::digitalRead },
            { "digitalWrite", &Uno::digitalWrite },
            { "micros", &Uno::micros },
            { "millis", &Uno::millis },
            { "noTone", &Uno::noTone },
            { "pinMode", &Uno::pinMode },
            { "tone", &Uno::tone },
        // $ Please set your function to use.
            { "", 0 }
        };
    // Creates function table.
        int c = sizeof(ftl) / sizeof(AoiBase::FunctionTable);
        m_functionTable = new AoiBase::FunctionTable[ c ];

        for( int i=0; i<c; i++ )
        {
            AoiBase::FunctionTable *ft = m_functionTable + i;
            ft->name = ftl[ i ].name;
            ft->pointer = ftl[ i ].pointer;
        }
    }
    /**
     * @fn Uno::~Uno( void )
     *
     * Destructor. Member variables are deleted.
     */
    Uno::~Uno( void )
    {
    }
    /**
     * @fn String Uno::className( void )
     *
     * @see bool Uno::className( void )
     */
    String Uno::className( void )
    {
        return String( "Uno" );
    }
    /**
     * @fn bool Uno::isExist( const String &function )
     *
     * @see bool AbstractBase::isExist( const String &function )
     */
    bool Uno::isExist( const String &function )
    {
        bool b = false;
        AoiBase::FunctionTable *ft = m_functionTable;

        while( ft->pointer )
        {
            if( ft->name==function )
            {
                b = true;
                break;
            }
            ft++;
        }

        return b;
    }
    /**
     * @fn String Uno::practice( StringList *args )
     *
     * @see String AbstractBase::practice( StringList *args )
     */
    String Uno::practice( StringList *args )
    {
        String s;
        AoiBase::FunctionTable *ft = m_functionTable;

        while( ft->pointer )
        {
            if( ft->name==args->value )
            {
            // Sets argument only.
                s = (*ft->pointer)( args+1 );
                break;
            }
            ft++;
        }

        return s;
    }
    /**
     * @fn String Uno::usages( void )
     *
     * @see String AbstractBase::usages( void )
     */
    String Uno::usages( void )
    {
        String s;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        r[ "className" ] = className();

        AoiBase::FunctionTable *ft = m_functionTable;
        JsonArray &a = r.createNestedArray( "methods" );

        while( ft->pointer )
        {
            JsonObject &o = a.createNestedObject();
            o[ "name" ] = ft->name;
            o[ "usage" ] = (*ft->pointer)( 0 );
            ft++;
        }
        r.prettyPrintTo( s );

        return s;
    }
    /**
     * @fn String Uno::analogRead( StringList *args )
     *
     * Reads the value from the specified analog pin.
     *
     * @param[in] args Reference to arguments.
     * @return int (0 to 1023).
     */
    String Uno::analogRead( StringList *args )
    {
        String s;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        switch( count(args) )
        {
            case 1:
                r[ "value" ] = ::analogRead( _atoi(0) );
                r.prettyPrintTo( s );
                break;
            default:
                s = usage( "analogRead pin" );
                break;
        }

        return s;
    }
    /**
     * @fn String Uno::analogWrite( StringList *args )
     *
     * Writes an analog value (PWM wave) to a pin.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Uno::analogWrite( StringList *args )
    {
        String s;
        uint8_t i = 0;
        int j = 0;

        switch( count(args) )
        {
            case 2:
                i = _atoi( 0 );
                j = _atoi( 1 );
                ::analogWrite( i, j );
                break;
            default:
                s = usage( "analogWrite pin 0-255" );
                break;
        }

        return s;
    }
    /**
     * @fn String Uno::delay( StringList *args )
     *
     * Pauses the program for the amount of time (in miliseconds) specified as
     * parameter.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Uno::delay( StringList *args )
    {
        String s;
        unsigned long l = 0;

        switch( count(args) )
        {
            case 1:
                l = _atoul( 0 );
                ::delay( l );
                break;
            default:
                s = usage( "delay 0-4294967295" );
                break;
        }

        return s;
    }
    /**
     * @fn String Uno::delayMicroseconds( StringList *args )
     *
     * Pauses the program for the amount of time (in microseconds) specified as
     * parameter.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Uno::delayMicroseconds( StringList *args )
    {
        String s;
        unsigned long l = 0;

        switch( count(args) )
        {
            case 1:
                l = _atoul( 0 );
                ::delayMicroseconds( l );
                break;
            default:
                s = usage( "delayMicroseconds 0-4294967295" );
                break;
        }

        return s;
    }
    /**
     * @fn String Uno::digitalRead( StringList *args )
     *
     * Reads the value from a specified digital pin, either HIGH or LOW.
     *
     * @param[in] args Reference to arguments.
     * @return HIGH or LOW.
     */
    String Uno::digitalRead( StringList *args )
    {
        String s;
        uint8_t i = 0;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        switch( count(args) )
        {
            case 1:
                i = ::digitalRead( _atoi(0) );
                if( i==HIGH )
                    r[ "value" ] = "HIGH";
                else
                    r[ "value" ] = "LOW";
                r.prettyPrintTo( s );
                break;
            default:
                s = usage( "digitalRead pin" );
                break;
        }

        return s;
    }
    /**
     * @fn String Uno::digitalWrite( StringList *args )
     *
     * Write a HIGH or a LOW value to a digital pin.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Uno::digitalWrite( StringList *args )
    {
        String s;
        uint8_t i = 0;

        switch( count(args) )
        {
            case 2:
                i = _atoi( 0 );
                if( _a(1)=="HIGH" )
                    ::digitalWrite( i, HIGH );
                else
                    ::digitalWrite( i, LOW );
                break;
            default:
                s = usage( "digitalWrite pin (HIGH|LOW)" );
                break;
        }

        return s;
    }
    /**
     * @fn String Uno::micros( StringList *args )
     *
     * Returns the number of microseconds since the Arduino board began running the
     * current program.
     *
     * @param[in] args Reference to arguments.
     * @return Number of microseconds since the program started.
     */
    String Uno::micros( StringList *args )
    {
        String s;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        switch( count(args) )
        {
            case 0:
                r[ "value" ] = ::micros();
                r.prettyPrintTo( s );
                break;
            default:
                s = usage( "micros" );
                break;
        }

        return s;
    }
    /**
     * @fn String Uno::millis( StringList *args )
     *
     * Returns the number of milliseconds since the Arduino board began running the
     * current program.
     *
     * @param[in] args Reference to arguments.
     * @return Number of milliseconds since the program started.
     */
    String Uno::millis( StringList *args )
    {
        String s;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        switch( count(args) )
        {
            case 0:
                r[ "value" ] = ::millis();
                r.prettyPrintTo( s );
                break;
            default:
                s = usage( "millis" );
                break;
        }

        return s;
    }
    /**
     * @fn String Uno::noTone( StringList *args )
     *
     * Stops the generation of a square wave triggered by tone().
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Uno::noTone( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
                ::noTone( _atoi(0) );
                break;
            default:
                s = usage( "noTone pin" );
                break;
        }

        return s;
    }
    /**
     * @fn String Uno::pinMode( StringList *args )
     *
     * Sets digital pin mode.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Uno::pinMode( StringList *args )
    {
        String s;
        String t;
        uint8_t i = 0;

        switch( count(args) )
        {
            case 2:
                i = _atoi( 0 );
                t = _a( 1 );
                if( t=="INPUT" )
                {
                    ::pinMode( i, INPUT );
                    break;
                }
                else if( t=="OUTPUT" )
                {
                    ::pinMode( i, OUTPUT );
                    break;
                }
                else if( t=="INPUT_PULLUP" )
                {
                    ::pinMode( i, INPUT_PULLUP );
                    break;
                }
            default:
                s = usage( "pinMode pin (INPUT|OUTPUT|INPUT_PULLUP)" );
                break;
        }

        return s;
    }
    /**
     * @fn String Uno::tone( StringList *args )
     *
     * Generates a square wave of the specified frequency.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Uno::tone( StringList *args )
    {
        String s;
        int i = 0;

        switch( count(args) )
        {
            case 2:
                i = _atoi( 0 );
                ::tone( i, _atoui(1) );
                break;
            case 3:
                i = _atoi( 0 );
                ::tone( i, _atoui(1), _atoul(2) );
                break;
            default:
                s = usage( "tone pin frequency (duration)" );
                break;
        }

        return s;
    }
}
#endif
