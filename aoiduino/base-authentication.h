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

#include "base-abstractbase.h"
/**
 * @namespace AoiBase
 * @brief Aoi base classes.
 */
namespace AoiBase
{
    using namespace AoiCore;
    /**
     * @class Authentication
     * @brief Provides authentication contents.
     *
     * This class provides authentication contents. This class is called after boot
     * loader or need authentication.
     */
    class Authentication :
        public AbstractBase
    {
    // members
    public:
        explicit Authentication( void );
        virtual ~Authentication( void );
    // members
    public:
        virtual bool basic( const String &prompt, const String &user, const String &password );
    // members
    protected:
        virtual String className( void );
        virtual bool isExist( const String &function );
        virtual String practice( StringList *args );
        virtual String usages( void );
    };
}
