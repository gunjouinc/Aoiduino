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

class Adafruit_NeoPixel;

#include "core-abstractcore.h"
/**
 * @namespace AoiUtil
 * @brief Aoi utility classes.
 */
namespace AoiUtil
{
    using namespace AoiCore;
    /**
     * @class NeoPixel
     * @brief Provides LED functions using Adafruit_NeoPixel.
     * @see https://github.com/adafruit/Adafruit_NeoPixel
     *
     * This class provides LED functions using Adafruit_NeoPixel.
     */
    class NeoPixel
    {
    // members
    public:
        explicit NeoPixel( void );
        virtual ~NeoPixel( void );
    // static members
    protected:
        // ^ Please set your function to use.
        static String neoPixelBegin( StringList *args );
        static String neoPixelClear( StringList *args );
        static String neoPixelEnd( StringList *args );
        static String neoPixelSetBrightness( StringList *args );
        static String neoPixelSetPixelColor( StringList *args );
        static String neoPixelShow( StringList *args );
        // $ Please set your function to use.
    // static variables
    private:
        /** Holds Adafruit_NeoPixel instance. */
        static Adafruit_NeoPixel *m_neoPixel;
    };
}
