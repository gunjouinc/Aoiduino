/******************************************************************************
**
** Copyright 2009-2024 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#pragma once

#include "core-abstractcore.h"
#include "base-arduinojson.h"
/** Application instance. */
#define aoi *this
/** Reference to product. */
namespace AoiProduct
{
    class Template;
}
/** Reference to core. */
namespace AoiCore
{
    class AbstractSerial;
#ifdef ARDUINO_M5STACK_Core2
    class AbstractBluetoothSerial;
#endif
}
/**
 * @namespace AoiBase
 * @brief Aoi base classes.
 */
namespace AoiBase
{
    using namespace AoiCore;
    /** Function pointer to practice. */
    typedef String (*AOIDUINOFUNC)( StringList* );
    /**
     * @class AbstractBase
     * @brief Provides application contents.
     *
     * This class provides application contents. Please inherit this class your
     * program.
     */
    class AbstractBase
    {
    // members
    public:
        explicit AbstractBase( void );
        virtual ~AbstractBase( void );
        // members
    public:
        /**
         * @fn String AbstractBase::className( void ) = 0
         *
         * Returns class name.
         *
         * @return Class name.
         */
        virtual String className( void ) = 0;
        /**
         * @fn bool AbstractCore::isExist( const String &function ) = 0
         *
         * Returns target class has function.
         *
         * @param[in] function Function name to check.
         * @return Returns true if has function, Otherwise returns false.
         */
        virtual bool isExist( const String &function ) = 0;
        /**
         * @fn String AbstractCore::practice( StringList *args ) = 0
         *
         * Returns practice result.
         *
         * @param[in] args Reference to string list.
         * @return Practice result.
         */
        virtual String practice( StringList *args ) = 0;
        /**
         * @fn String AbstractCore::usages( void ) = 0
         *
         * Returns all usages in all functions. Please use @ as delimiter.
         *
         * @return All usage.
         */
        virtual String usages( void ) = 0;
    // members
    public:
        virtual bool begin( StringList *args, InterfaceType type = AoiCore::_Serial );
        virtual int bytesAvailable( void );
        virtual void end( void );
        virtual void flush( void );
        virtual InterfaceType interfaceType( void ) const;
        virtual bool isBegan( void );
        virtual String read( void );
        virtual StringList* rcScript( const String &index );
        virtual void setTemplate( AoiProduct::Template *product );
        virtual int write( const String &data );
    // members
    protected:
        virtual void afterKeyInput( int input, String *buffer );
        virtual bool beforeKeyInput( int input, String *buffer );
        virtual void internalLoop( void );
        virtual int operator~( void );
        virtual int operator!( void );
        virtual int operator>>( String *data );
        virtual int operator<<( const String &data );
        virtual int operator<=( const String &data );
    // static members
    protected:
        static void handleTemplate( void *obj );
    // core variables
    protected:
        /** Holds AbstructSerial. */
        static AbstractSerial *m_abstractSerial;
#ifdef ARDUINO_M5STACK_Core2
        /** Holds AbstructBluetoothSerial. */
        static AbstractBluetoothSerial *m_abstractBluetoothSerial;
#endif
    // variables
    protected:
        /** Holds key code. */
        long m_keyCode = 0;
    // variables
    private:
        /** Holds interface type. */
        static InterfaceType m_interfaceType;
        /* Holds applicationHandler */
        static AoiProduct::Template *m_template;
        /** Holds web server. */
    };
    /**
     * @struct ClassTable
     * @brief Holds AbstractBase pointer.
     *
     * Holds AbstractBase pointer. This struct will be used for function pointer.
     */
    struct ClassTable
    {
        /** Class pointer */
        AbstractBase *pointer;
    };
    /**
     * @struct Function table.
     *
     * Provides function table that is used by application.
     */
    struct FunctionTable
    {
        /** Function name to call. */
        String name;
        /** Function pointer to practice. */
        AOIDUINOFUNC pointer;
    };
}
