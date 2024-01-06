/******************************************************************************
**
** Copyright 2009-2024 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#ifdef ARDUINO_M5STACK_Core2
#pragma once

#include "core-abstractcore.h"
/**
 * @namespace AoiCore
 * @brief Aoi core classes.
 */
namespace AoiCore
{
    /**
     * @class AbstractBluetoothSerial
     * @brief Provides bluetooth serial abstract class.
     *
     * Provides bluetooth serial abstract class. Inherit this class to add your original
     * application.
     */
    class AbstractBluetoothSerial :
        public AbstractCore
    {
    // members
    public:
        explicit AbstractBluetoothSerial( void );
        virtual ~AbstractBluetoothSerial( void );
    // members
    public:
        bool begin( StringList *args );
        int bytesAvailable( void );
        void end( void );
        void flush( void );
        bool isBegan( void );
        String read( void );
        int readByte( void );
        void setTimeout( int msec = 1 );
        int timeout( void ) const;
        int write( const String &data );
    // variables
    private:
        /** Holds status. */
        static bool m_boolBegan;
        /** Holds timeout. */
        static int m_intTimeout;
    };
}
#endif
