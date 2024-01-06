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

/** HTTP buffer size, include header */
#define _AOIUTIL_HTTP_BUFFER_SIZE_ (1024 * 2)

class Client;

#include "core-abstractcore.h"
/**
 * @namespace AoiUtil
 * @brief Aoi utility classes.
 */
namespace AoiUtil
{
    using namespace AoiCore;
    /**
     * @class Http
     * @brief Provides HTTP client functions.
     *
     * This class provides HTTP client.
     */
    class Http
    {
    // members
    public:
        explicit Http( void );
        virtual ~Http( void );
    // static members
    protected:
        // ^ Please set your function to use.
        static String httpGet( StringList *args );
        static String httpPost( StringList *args );
        // $ Please set your function to use.
    // static members
    protected:
        static String requestBodyFooterInPut( const String &boundary );
        static String requestBodyHeaderInPut( const String &boundary, const String &name, const String &value, int *size );
        static String response( int timeout = 30*1000 );
        static unsigned int responseRaw( char *buf, unsigned int size );
    // static variables
    protected:
        /** Holds Client instance. */
        static Client *http;
    };
}
