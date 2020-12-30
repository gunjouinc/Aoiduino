/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#include "base-shell.h"
#include "base-bootloader.h"
/*
 * Base class tables, Please set your classes below this line.
 */
 #include "avr-uno.h"
 #include "esp-esp32.h"
 #include "esp-m5stack.h"
 #include "spresense-ast.h"
/**
 * @namespace AoiBase
 * @brief Aoi base classes.
 */
namespace AoiBase
{
    /**
     * @fn BootLoader::BootLoader( void )
     *
     * Constructor. Member variables are initialized.
     */
    BootLoader::BootLoader( void )
    {
        m_classTable = 0;
    }
    /**
     * @fn BootLoader::~BootLoader( void )
     *
     * Destructor. Member variables are deleted.
     */
    BootLoader::~BootLoader( void )
    {
        int i = 0;
        if( m_classTable )
        {
            while( (m_classTable+i)->pointer )
            {
                delete (m_classTable+i)->pointer;
                i++;
            }
        }

        delete [] m_classTable;
    }
    /**
     * @fn bool BootLoader::begin( StringList *args, InterfaceType type = _Serial )
     *
     * Sets interface type and begin interface.
     *
     * @param[in] args Interface parameter.
     * @param[in] type Interface type.
     */
    bool BootLoader::begin( StringList *args, InterfaceType type )
    {
        return AbstractBase::begin( args, type );
    }
    /**
     * @fn ClassTable* BootLoader::classTable( void )
     *
     * Returns boot loader class table.
     */
    ClassTable* BootLoader::classTable( void )
    {
        return m_classTable;
    }
    /**
     * @fn void BootLoader::end( void )
     *
     * This method is called when global setup method is all done.
     */
    void BootLoader::end( void )
    {
    }
    /**
     * @fn void BootLoader::load( void )
     *
     * Loads class table.
     */
    void BootLoader::load( void )
    {
        int i = 0;
    // Deletes old instance.
        if( m_classTable )
        {
            while( (m_classTable+i)->pointer )
            {
                delete (m_classTable+i)->pointer;
                i++;
            }
        }
        delete [] m_classTable;
    // Sets new instance.
        ClassTable ctl[] =
        {
        /*
         * Base class tables, Please set your classes below this line.
         */
            new AoiBase::Shell(),
// Uno
#ifdef ARDUINO_AVR_UNO
            new AoiAvr::Uno(),
#endif
// Esp32
#ifdef ESP32
            new AoiEsp::Esp32(),
#endif
#ifdef ARDUINO_M5Stack_Core_ESP32
            new AoiEsp::M5Stack(),
#endif
// Spresense
#ifdef ARDUINO_spresense_ast
            new AoiSpresense::Ast(),
#endif
            0
        };
    // Creates class table to avoid kernel panic.
        int c = sizeof(ctl) / sizeof(ClassTable);

        m_classTable = new ClassTable[ c ];
        for( i=0; i<c; i++ )
        {
            ClassTable *ct = m_classTable + i;
            ct->pointer = ctl[ i ].pointer;
        }
    }
    /**
     * @fn String BootLoader::className( void )
     *
     * @see bool AbstractBase::className( void )
     */
    String BootLoader::className( void )
    {
        return String( "BootLoader" );
    }
    /**
     * @fn bool BootLoader::isExist( const String &function )
     *
     * @see bool AbstractBase::isExist( const String &function )
     */
    bool BootLoader::isExist( const String &function )
    {
        return false;
    }
    /**
     * @fn String BootLoader::practice( StringList *args )
     *
     * @see String AbstractBase::practice( StringList *args )
     */
    String BootLoader::practice( StringList *args )
    {
        return String();
    }
    /**
     * @fn String BootLoader::usages( void )
     *
     * @see String AbstractBase::usages( void )
     */
    String BootLoader::usages( void )
    {
        return String();
    }
}
