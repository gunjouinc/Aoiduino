/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
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
            { "analogRead", &Arduino::analogRead },
            { "analogWrite", &Arduino::analogWrite },
            { "delay", &Arduino::delay },
            { "delayMicroseconds", &Arduino::delayMicroseconds },
            { "digitalRead", &Arduino::digitalRead },
            { "digitalWrite", &Arduino::digitalWrite },
            { "micros", &Arduino::micros },
            { "millis", &Arduino::millis },
            { "noTone", &Arduino::noTone },
            { "pinMode", &Arduino::pinMode },
            { "tone", &Arduino::tone },
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
        return Arduino::isExist( function, m_functionTable );
    }
    /**
     * @fn String Uno::practice( StringList *args )
     *
     * @see String AbstractBase::practice( StringList *args )
     */
    String Uno::practice( StringList *args )
    {
        return Arduino::practice( args, m_functionTable );
    }
    /**
     * @fn String Uno::usages( void )
     *
     * @see String AbstractBase::usages( void )
     */
    String Uno::usages( void )
    {
        return Arduino::usages( m_functionTable );
    }
}
#endif
