/******************************************************************************
**
** Copyright 2009-2023 Gunjou Inc. All rights reserved.
** Contact: Gunjou Inc. (information@gunjou.co.jp)
**
** This software is released under the MIT License.
** https://github.com/gunjouinc/Aoiduino/blob/master/LICENSE
**
******************************************************************************/
#pragma once

#include "core-abstractcore.h"
#if defined(ESP8266) || defined(ESP32)
#include <Espalexa.h>
#else
class Espalexa;
class EspalexaDevice;
#endif
/**
 * @namespace AoiUtil
 * @brief Aoi utility classes.
 */
namespace AoiUtil
{
    using namespace AoiCore;
    /**
     * @class Alexa
     * @brief Provides Alexa functions using Espalexa.
     * @see https://github.com/Aircoookie/Espalexa
     *
     * This class provides Alexa functions using Espalexa.
     */
    class Alexa
    {
    // members
    public:
        explicit Alexa( void );
        virtual ~Alexa( void );
    // static members
    protected:
        // ^ Please set your function to use.
        static String alexaAddDevice( StringList *args );
        static String alexaBegin( StringList *args );
        static String alexaGetValue( StringList *args );
        static String alexaLoop( StringList *args );
        // $ Please set your function to use.
    // static variables
    private:
        /** Holds Alexa instance. */
        static Espalexa *espalexa;
    };
}
