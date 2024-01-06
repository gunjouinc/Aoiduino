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

class IRrecv;
class IRsend;

#include "core-abstractcore.h"
/**
 * @namespace AoiUtil
 * @brief Aoi utility classes.
 */
namespace AoiUtil
{
    using namespace AoiCore;
    /**
     * @class IRRemote
     * @brief Provides IR send/receive functions using IRremoteESP8266.
     * @see https://github.com/crankyoldgit/IRremoteESP8266
     *
     * This class provides IR send/receive functions using IRremoteESP8266.
     */
    class IRRemote
    {
    // members
    public:
        explicit IRRemote( void );
        virtual ~IRRemote( void );
    // static members
    protected:
        // ^ Please set your function to use.
        static String irReceiveBegin( StringList *args );
        static String irReceiveRaw( StringList *args );
        static String irSendBegin( StringList *args );
        static String irSendRaw( StringList *args );
        // $ Please set your function to use.
    // static variables
    private:
        /** Holds IRrecv instance. */
        static IRrecv *irReceive;
        /** Holds IRsend instance. */
        static IRsend *irSend;
    };
}
