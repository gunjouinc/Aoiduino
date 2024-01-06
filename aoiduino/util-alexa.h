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

class Espalexa;

#include "core-abstractcore.h"
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
        static String alexaGetRGB( StringList *args );
        static String alexaGetValue( StringList *args );
        static String alexaLoop( StringList *args );
        // $ Please set your function to use.
    // static variables
    private:
        /** Holds Alexa instance. */
        static Espalexa *espalexa;
    };
}
