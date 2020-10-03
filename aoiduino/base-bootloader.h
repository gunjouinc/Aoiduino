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
     * @class BootLoader
     * @brief Provides boot loader function.
     *
     * This class is loaded when boot first time.
     */
    class BootLoader :
        public AbstractBase
    {
    // members
    public:
        explicit BootLoader( void );
        virtual ~BootLoader( void );
    // members
    public:
        virtual bool begin( StringList *args, InterfaceType type = _Serial );
        virtual ClassTable* classTable( void );
        virtual void end( void );
        virtual void load( void );
    // members
    protected:
        virtual String className( void );
        virtual bool isExist( const String &function );
        virtual String practice( StringList *args );
        virtual String usages( void );
    // variables
    private:
        /** Holds class tables. */
        ClassTable *m_classTable;
    };
}
