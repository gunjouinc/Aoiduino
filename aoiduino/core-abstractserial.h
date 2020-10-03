/******************************************************************************
**
** Copyright 2009-2020 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#pragma once

#include "core-abstractcore.h"
/**
 * @namespace AoiCore
 * @brief Aoi core classes.
 */
namespace AoiCore
{
    /**
     * @class AbstractSerial
     * @brief Provides serial abstract class.
     *
     * Provides serial abstract class. Inherit this class to add your original
     * application.
     */
    class AbstractSerial :
        public AbstractCore
    {
    // members
    public:
        explicit AbstractSerial( void );
        virtual ~AbstractSerial( void );
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
