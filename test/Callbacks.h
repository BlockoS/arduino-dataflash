/*
Simple Unit test framework.
Based on UnitTest++ by Noel Llopis and Charles Nicholson.
This is basically a stripped down version meant to be run on a
microcontroller.

History:

Version 0.1, 2011

Copyright (C) 2011 by Vincent Cruz.
cruz.vincent@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "Infos.h"

/**
 * @addtogroup DummyUnitTest
 * @{
 **/
namespace Dummy
{
    /**
     * CHECK failure notification interface.
     **/
    class CheckFailCallbackInterface
    {
        public:
            /** Destructor. **/
            virtual ~CheckFailCallbackInterface() {};
            /**
             * CHECK failure notification.
             * @param [in] expected : expected value string.
             * @param [in] value    : tested value string.
             * @param [in] infos    : test informations.
             **/
            virtual void Notify(char const* expected, char const* value, Infos const& infos) = 0;
    };
 
    /**
     * CHECK failure notification helper.
     **/
    template <class T>
    class CheckFailCallback : public CheckFailCallbackInterface
    {
        public:
            typedef void (T::*TNotifyProc) (char const*, char const*, Infos const&);
        
            /** Constructor. **/
            CheckFailCallback(T* handler, TNotifyProc notify);
            /** Destructor. **/
            virtual ~CheckFailCallback() {}
            
            /**
             * CHECK failure notification. 
             * @param [in] expected : expected value string.
             * @param [in] value    : tested value string.
             * @param [in] infos    : test informations.
             **/
            virtual void Notify(char const* expected, char const* value, Infos const& infos);
            
        protected:
            T* m_handler; /**< Handler instance. **/
            TNotifyProc m_notify; /**< Handler method to be called on failure notification. **/
    };
    
    /** Constructor. **/
    template <class T>
    CheckFailCallback<T>::CheckFailCallback(T* handler, TNotifyProc notify)
        : m_handler(handler)
        , m_notify(notify)
    {}
    
    /**
     * CHECK failure notification. 
     * @param [in] expected : expected value string.
     * @param [in] value    : tested value string.
     * @param [in] infos    : test informations.
     **/
    template <class T>
    void CheckFailCallback<T>::Notify(char const* expected, char const* value, Infos const& infos)
    {
        (m_handler->*(m_notify)) (expected, value, infos);
    }
};
/** @} **/

#endif // CALLBACKS_H