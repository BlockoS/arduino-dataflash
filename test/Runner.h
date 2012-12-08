/**************************************************************************//**
 * @file Runner.h
 *
 * @par Copyright: 
 * - Copyright (C) 2011 by Vincent Cruz.
 * All rights reserved.
 *
 * @author Vincent Cruz cruz.vincent@gmail.com
 *
 * @par Description:
 *  Simple Unit test framework. @n
 *  Based on UnitTest++ by Noel Llopis and Charles Nicholson. @n
 *  This is basically a stripped down version meant to be run on a
 *  microcontroller.
 *
 * @par History:
 * - Version 0.1, 28 Dec 2011.
 *
 * @par Licence: GPLv3
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version. @n
 * @n
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. @n
 * @n
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/
#ifndef RUNNER_H
#define RUNNER_H

#include "Utils.h"
#include "Result.h"
#include "Callbacks.h"

/**
 * @addtogroup DummyUnitTest
 * @{
 **/
namespace Dummy
{
    class Test;
    
    /**
	 * @brief The unit test manager. 
     * It basically holds a list of tests and execute them
     * in the order in which they were declared.
	 **/
    class Runner
    {
      public:
        /** Destructor **/
        ~Runner();

		/**
		 * Process every test declared.
         * @param [in] notify : Notification callback (optional).
		 * @return Tests result.
		 **/
        Result Run(TestNotificationInterface* notify=0);

        /**
         * CHECK failure notification. 
         * @param [in] expected : expected value string.
         * @param [in] value    : tested value string.
         * @param [in] infos    : test informations.
         **/
        void OnCheckFailed(char const* expected, char const* value, Infos const& infos);
        
		/**
		 * Add a test to the suite.
		 * @param [in] elmnt : pointer to the test to be added.
		 **/
        void Add(Test *elmnt);
        
		/** Retrieve the unit test manager instance. **/
		static Runner& Instance();

      private:
        /** Constructor. **/
        Runner();
        /** Copy constructor. **/
        Runner(const Runner&);
        /** Copy operator. **/
        Runner& operator=(const Runner&);

      protected:
        Result m_result; /** Test campaign results. **/
        
        TestNotificationInterface* m_notify; /** Test notification callbacks. **/
        
        Test* m_head; /**< First element of the test list. **/
        Test* m_tail; /**< Last element of the test list. **/
    };

    /**
     * Check if the provided value is equal to the expected one and
     * notify if the test fails.
     **/
	template <typename T>
	bool Eval(T const& expected, T const &value, Dummy::Infos const& infos)
	{
		if(!IsEqual(expected, value))
		{
            char expectedStr[MAX_VALUE_STRING_SIZE];
            char valueStr[MAX_VALUE_STRING_SIZE];
		
            Stringify(expected, expectedStr, MAX_VALUE_STRING_SIZE);
            Stringify(value, valueStr, MAX_VALUE_STRING_SIZE);
            
            Runner::Instance().OnCheckFailed(expectedStr, valueStr, infos);
            return false;
		}
        return true;
	}
    
    /**
     * Check if the provided value is equal to the expected one and
     * notify if the test fails.
     **/
    bool Eval(char const* expected, char const* value, Dummy::Infos const& infos);
};

/**
 * Get current test suite name.
 * @return Test suite name.
 **/
const char* GetSuiteName();
/** @} **/
#endif // RUNNER_H