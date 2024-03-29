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
#include "core-util.h"
#include "core-strings.h"
#include "base-arduinojson.h"
/**
 * @namespace AoiProduct
 * @brief Aoi product classes.
 */
namespace AoiProduct
{
    using namespace AoiCore;
    /**
     * @class Tamplate
     * @brief Provides product template.
     *
     * This class provides product template. Functions are used in internal loop.
     */
    class Template
    {
    // members
    public:
        explicit Template( void );
        virtual ~Template( void );
    // members
    public:
        virtual String setup( void );
        virtual String loop( void );
    };
}
