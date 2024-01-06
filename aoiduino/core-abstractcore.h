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

#include "core-global.h"
#include "core-enum.h"
#include "core-strings.h"
#include "core-util.h"
#include <Arduino.h>
/**
 * @namespace AoiCore
 * @brief Aoi core classes.
 */
namespace AoiCore
{
    /**
     * @class AbstractCore
     * @brief Provides core abstract class.
     *
     * Provides core abstract class. Use member function to make your original
     * application.
     */
    class AbstractCore
    {
    // members
    public:
        explicit AbstractCore( void );
        virtual ~AbstractCore( void );
    // members
    public:
        /**
         * @fn bool AbstractCore::begin( StringList *args )
         *
         * Implement begin code of interface.
         *
         * @param[in] host Host name, ip, subnet and etc.
         * @param[in] post Port number in host.
         * @return Returns true, If this method succeeded. Otherwise returns false.
         */
        virtual bool begin( StringList *args ) = 0;
        /**
         * @fn int AbstractCore::bytesAvailable( void ) const
         *
         * Returns data available status.
         *
         * @return Return byte count if data is available, Otherwise returns 0.
         */
        virtual int bytesAvailable( void ) = 0;
        /**
         * @fn void AbstractCore::end( void )
         *
         * Implement end code of interface.
         */
        virtual void end( void ) = 0;
        /**
         * @fn void AbstractCore::flush( void )
         *
         * Implement flush code of interface.
         */
        virtual void flush( void ) = 0;
        /**
         * @fn bool AbstractCore::isBegan( void )
         *
         * Returns server status.
         *
         * @return Returns true if began, Otherwise returns false.
         */
        virtual bool isBegan( void ) = 0;
        /**
         * @fn String AbstractCore::read( void )
         *
         * Reads string data from interface.
         *
         * @return Read data from interface.
         */
        virtual String read( void ) = 0;
        /**
         * @fn int AbstractCore::readByte( void )
         *
         * Reads incoming data from interface.
         *
         * @return Read incoming data from interface. If there is no data, Returns -1.
         */
        virtual int readByte( void ) = 0;
        /**
         * @fn void AbstractCore::setTimeout( int msec = 1 )
         *
         * Sets timeout to read/write on interface (msec).
         *
         * @param[in] msec Timeout while using read/write method.
         */
        virtual void setTimeout( int msec = 1 ) = 0;
        /**
         * @fn int AbstractCore::timeout( void ) const
         *
         * Returns timeout to read/write (msec).
         *
         * @return Timeout while using read/write method.
         */
        virtual int timeout( void ) const = 0;
        /**
         * @fn int AbstractCore::write( const String &data )
         *
         * Write string data to interface.
         *
         * @param[in] data Text to write.
         * @return Return Writen byte count.
         */
        virtual int write( const String &data ) = 0;
    // members
    protected:
        virtual int bytesAvailable( int msec );
    };
}
