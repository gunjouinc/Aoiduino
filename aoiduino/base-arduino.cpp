/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (aoiduino@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/masaomiura/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#include "base-arduino.h"
/**
* @namespace AoiBase
* @brief Aoi base classes.
 */
namespace AoiBase
{
// Static variables.
    FunctionTable *Arduino::m_functionTable = 0;
    /**
     * @fn Arduino::Arduino( void )
     *
     * Constructor. Member variables are initialized.
     */
    Arduino::Arduino( void )
    {
        if( m_functionTable )
            return;
    // Sets function table, If there is no instance.
        FunctionTable ftl[] =
        {
        // ^ Please set your function to use.
            { ">", &Arduino::create },
            { ">>", &Arduino::append },
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
            { "sed", &Arduino::sed },
            { "tone", &Arduino::tone },
            { "yield", &Arduino::yield },
        // $ Please set your function to use.
            { "", 0 }
        };
    // Creates function table to avoid kernel panic.
        uint8_t c = sizeof(ftl) / sizeof(FunctionTable);
        m_functionTable = functionTable( ftl, c );
    }
    /**
     * @fn Arduino::~Arduino( void )
     *
     * Destructor. Member variables are deleted.
     */
    Arduino::~Arduino( void )
    {
    }
    /**
     * @fn String Arduino::className( void )
     *
     * @see bool Arduino::className( void )
     */
    String Arduino::className( void )
    {
        return String( "Arduino" );
    }
    /**
     * @fn bool Arduino::isExist( const String &function )
     *
     * @see bool AbstractBase::isExist( const String &function )
     */
    bool Arduino::isExist( const String &function )
    {
        return isExist( function, m_functionTable );
    }
    /**
     * @fn String Arduino::practice( StringList *args )
     *
     * @see String AbstractBase::practice( StringList *args )
     */
    String Arduino::practice( StringList *args )
    {
        return practice( args, m_functionTable );
    }
    /**
     * @fn String Arduino::usages( void )
     *
     * @see String AbstractBase::usages( void )
     */
    String Arduino::usages( void )
    {
        return usages( m_functionTable );
    }
    /**
     * @fn FunctionTable* Arduino::functionTable( FunctionTable *table, uint8_t size )
     *
     * Returns new function table. Please delete table by caller.
     *
     * @param[in] table Function table to create.
     * @param[in] size Table size.
     */
    FunctionTable* Arduino::functionTable( FunctionTable *table, uint8_t size )
    {
        FunctionTable *ftl = new FunctionTable[ size ];

        for( int i=0; i<size; i++ )
        {
            FunctionTable *ft = ftl + i;
            ft->name = table[ i ].name;
            ft->pointer = table[ i ].pointer;
        }

        return ftl;
    }    /**
     * @fn bool Arduino::isExist( const String &function, FunctionTable *table )
     *
     * @param[in] table Function table to check.
     * @see bool AbstractBase::isExist( const String &function )
     */
    bool Arduino::isExist( const String &function, FunctionTable *table )
    {
        bool b = false;
        FunctionTable *ft = table;

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
     * @fn String Arduino::practice( StringList *args, FunctionTable *table )
     *
     * @param[in] table Function table to check.
     * @see String AbstractBase::practice( StringList *args )
     */
    String Arduino::practice( StringList *args, FunctionTable *table )
    {
        String s;
        FunctionTable *ft = table;

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
     * @fn String Arduino::usages( FunctionTable *table )
     *
     * @param[in] table Function table to check.
     * @see String AbstractBase::usages( void )
     */
    String Arduino::usages( FunctionTable *table )
    {
        String s;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        r[ "className" ] = className();

        FunctionTable *ft = table;
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
     * @fn String Arduino::prettyPrintTo( const String &key, unsigned int value )
     *
     * Returns JSON string from key and value.
     *
     * @param[in] key Key of JSON.
     * @param[in] value Value of JSON.
     * @return JSON string like { "key": value }.
     */
    String Arduino::prettyPrintTo( const String &key, unsigned int value )
    {
        String s;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        r[ key ] = value;
        r.prettyPrintTo( s );

        return s;
    }
    /**
     * @fn String Arduino::prettyPrintTo( const String &key, const String &value )
     *
     * Returns JSON string from key and value.
     *
     * @param[in] key Key of JSON.
     * @param[in] value Value of JSON.
     * @return JSON string like { "key": "value" }.
     */
    String Arduino::prettyPrintTo( const String &key, const String &value )
    {
        String s;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        r[ key ] = value;
        r.prettyPrintTo( s );

        return s;
    }
    /**
     * @fn String Arduino::analogRead( StringList *args )
     *
     * Reads the value from the specified analog pin.
     *
     * @param[in] args Reference to arguments.
     * @return int (0 to 1023).
     */
    String Arduino::analogRead( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 1:
                s = prettyPrintTo( "value", ::analogRead(_atoi(0)) );
                break;
            default:
                s = usage( "analogRead pin" );
                break;
        }

        return s;
    }
    /**
     * @fn String Arduino::analogWrite( StringList *args )
     *
     * Writes an analog value (PWM wave) to a pin.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Arduino::analogWrite( StringList *args )
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
     * @fn String Arduino::delay( StringList *args )
     *
     * Pauses the program for the amount of time (in miliseconds) specified as
     * parameter.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Arduino::delay( StringList *args )
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
     * @fn String Arduino::delayMicroseconds( StringList *args )
     *
     * Pauses the program for the amount of time (in microseconds) specified as
     * parameter.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Arduino::delayMicroseconds( StringList *args )
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
     * @fn String Arduino::digitalRead( StringList *args )
     *
     * Reads the value from a specified digital pin, either HIGH or LOW.
     *
     * @param[in] args Reference to arguments.
     * @return HIGH or LOW.
     */
    String Arduino::digitalRead( StringList *args )
    {
        String s;
        uint8_t i = 0;

        switch( count(args) )
        {
            case 1:
                i = ::digitalRead( _atoi(0) );
                if( i==HIGH )
                    s = prettyPrintTo( "value" , "HIGH" );
                else
                    s = prettyPrintTo( "value" , "LOW" );
                break;
            default:
                s = usage( "digitalRead pin" );
                break;
        }

        return s;
    }
    /**
     * @fn String Arduino::digitalWrite( StringList *args )
     *
     * Write a HIGH or a LOW value to a digital pin.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Arduino::digitalWrite( StringList *args )
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
     * @fn String Arduino::echo( StringList *args )
     *
     * Echos message.
     *
     * @return Message string.
     */
    String Arduino::echo( StringList *args )
    {
        String s;
        int c = count( args );

        if( c<1 )
            s = usage( "echo .+" );
        else
        {
            String t = join( args, STR_SPACE );
            s = prettyPrintTo( "value" , t );
        }

        return s;
    }
    /**
     * @fn String Arduino::micros( StringList *args )
     *
     * Returns the number of microseconds since the Arduino board began running the
     * current program.
     *
     * @param[in] args Reference to arguments.
     * @return Number of microseconds since the program started.
     */
    String Arduino::micros( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            case 0:
                s = prettyPrintTo( "value" , ::micros() );
                break;
            default:
                s = usage( "micros" );
                break;
        }

        return s;
    }
    /**
     * @fn String Arduino::millis( StringList *args )
     *
     * Returns the number of milliseconds since the Arduino board began running the
     * current program.
     *
     * @param[in] args Reference to arguments.
     * @return Number of milliseconds since the program started.
     */
    String Arduino::millis( StringList *args )
    {
        String s;
        DynamicJsonBuffer json;
        JsonObject &r = json.createObject();

        switch( count(args) )
        {
            case 0:
                s = prettyPrintTo( "value" , ::millis() );
                break;
            default:
                s = usage( "millis" );
                break;
        }

        return s;
    }
    /**
     * @fn String Arduino::noTone( StringList *args )
     *
     * Stops the generation of a square wave triggered by tone().
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Arduino::noTone( StringList *args )
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
     * @fn String Arduino::pinMode( StringList *args )
     *
     * Sets digital pin mode.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Arduino::pinMode( StringList *args )
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
     * @fn String Arduino::sed( StringList *args )
     *
     * Replace content and output.
     *
     * @param[in] args Reference to arguments.
     * @return Replaced string.
     */
    String Arduino::sed( StringList *args )
    {
        String s;
        StringList *sl;
        int c = count( args );

        if( c<2 )
            s = usage( "sed before/after target" );
        else
        {
            sl = split( _a(0), "/" );
            if( count(sl)!=2 )
            {
                delete [] sl;
                return sed( 0 );
            }
        // restore argument after replacement pattern
            String t;
            for( int i=1; i<c; i++ )
            {
                if( 1<i )
                    t += STR_SPACE;
                t += _a( i );
            }
        // Replace before to after
            t.replace( (sl+0)->value, (sl+1)->value );
            s = prettyPrintTo( "value" , t );
            delete [] sl;
        }

        return s;
    }
    /**
     * @fn String Arduino::tone( StringList *args )
     *
     * Generates a square wave of the specified frequency.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Arduino::tone( StringList *args )
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
    /**
     * @fn tring Arduino::yield( StringList *args )
     *
     * Passes control to other tasks when called.
     *
     * @param[in] args Reference to arguments.
     * @return Empty string.
     */
    String Arduino::yield( StringList *args )
    {
        String s;
        int start = 0;

        switch( count(args) )
        {
            case 1:
                start = ::millis() / 1000;
                while( true )
                {
                    ::yield();
                    if( _atoi(0)<((::millis()/1000)-start) )
                        break;
                }
                break;
            default:
                s = usage( "yield [0-9]+" );
                break;
        }

        return s;
    }
    /**
     * @fn String Arduino::append( StringList *args )
     *
     * Append value on current device.
     *
     * @param[in] args Reference to arguments.
     * @return value string.
     */
    String Arduino::append( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            default:
                s = usage( STR_COMMAND_NOT_INPLEMENTED );
                break;
        }

        return s;
     }
    /**
     * @fn String Arduino::create( StringList *args )
     *
     * Create value on current device.
     *
     * @param[in] args Reference to arguments.
     * @return value string.
     */
    String Arduino::create( StringList *args )
    {
        String s;

        switch( count(args) )
        {
            default:
                s = usage( STR_COMMAND_NOT_INPLEMENTED );
                break;
        }

        return s;
    }
}
