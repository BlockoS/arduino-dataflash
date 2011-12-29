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
#include "Test.h"

/**
 * @addtogroup DummyUnitTest
 * @{
 **/
namespace Dummy
{
    /** Constructor **/
    Runner::Runner()
      : m_onCheckFailed(0)
      , m_head(0)
      , m_tail(0)
    {}

    /** Copy constructor **/
    Runner::Runner(const Runner&)
    {}
        
    /** Destructor **/
    Runner::~Runner()
    {}

    /** Copy operator **/
    Runner& Runner::operator=(const Runner&)
    {
      return *this;
    }

    /** Retrieve the unit test manager instance. **/
    Runner& Runner::Instance()
    {
      static Runner instance;
      return instance;
    }

    /**
     * Process every test declared.
     * @param [in] onCheckFailed : CHECK failure notification callback.
     * @return Tests result.
     **/
    Result Runner::Run(CheckFailCallbackInterface* onCheckFailed)
    {
      m_onCheckFailed = onCheckFailed;
      m_result.Reset();
      for(Test* it=m_head; it!=0; it=it->m_next, ++m_result.total)
      {
        it->m_success = true;
        it->Run();
        if(!it->m_success)
        {
            ++m_result.failed;
        }
      }
      return m_result;
    }

    /**
     * CHECK failure notification. 
     * @param [in] expected : expected value string.
     * @param [in] value    : tested value string.
     * @param [in] infos    : test informations.
     **/
    void Runner::OnCheckFailed(char const* expected, char const* value, Infos const& infos)
    {
        ++m_result.error;
        if(m_onCheckFailed)
        {
            m_onCheckFailed->Notify(expected, value, infos);
        }
    }

    /**
	 * Add a test to the suite.
     * @param [in] elmnt : pointer to the test to be added.
	 **/
    void Runner::Add(Test *elmnt)
    {
      elmnt->m_previous = m_tail;
      elmnt->m_next = 0;
      if(m_tail != 0)
      {
        m_tail->m_next = elmnt;
      }
      m_tail = elmnt;
      if(m_head == 0)
      {
        m_head = elmnt;
      }
    }
    
    /**
     * Check if the provided value is equal to the expected one and
     * notify if the test fails.
     **/
    bool Eval(char const* expected, char const* value, Dummy::Infos const& infos)
    {
       	if(!IsEqual(expected, value))
		{   
            Runner::Instance().OnCheckFailed(expected, value, infos);
            return false;
		}
        return true;
    }
};

/**
 * Get current test suite name.
 * @return Test suite name.
 **/
const char* GetSuiteName()
{ 
    return "";
}

/** @} **/