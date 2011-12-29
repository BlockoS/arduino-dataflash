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
#ifndef MACROS_H
#define MACROS_H

/**
 * @addtogroup DummyUnitTest
 * @{
 **/
 
/**
 * @def CHECK(expected, value)
 * Check \a value against \a expected. The test is considered to have failed if the \a value and \a expected
 * don't match.
 **/
#define CHECK(expected, value) \
do { \
    if(!Dummy::Eval(expected, value, Dummy::Infos(__FILE__, GetSuiteName(), m_name, __LINE__))) \
    { m_success = false; } \
} while(0);

/**
 * @def TEST(TestName)
 * Declares a new test.
 **/
#define TEST(TestName) \
class TestName : public Dummy::Test \
{ \
  public: \
    TestName() : Dummy::Test(#TestName) { Dummy::Runner::Instance().Add(this); } \
    ~TestName() {} \
    virtual void Run(); \
}; \
static TestName TestName ## instance; \
void TestName::Run() \

/**
 * @def TEST_FIXTURE(TestName, Fixture)
 * Declares a new test with fixture. The Fixture public and protected members can be
 * accessed by the test. It must also implement a Setup() and TearDown() methods.
 * Setup() and TearDown() will be called respectively before and after the test.
 **/
#define TEST_FIXTURE(TestName, Fixture) \
class TestName : public Dummy::Test, public Fixture \
{ \
  public: \
    TestName() : Dummy::Test(#TestName) { Dummy::Runner::Instance().Add(this); } \
    ~TestName() {} \
    virtual void Run() \
    { \
      Fixture::Setup(); \
      Execute(); \
      Fixture::TearDown(); \
    } \
    void Execute(); \
}; \
static TestName TestName ## instance; \
void TestName::Execute() \

/**
 * @def SUITE(SuiteName)
 * A test suite is basically a namespace. You can use it to group tests.
 **/
#define SUITE(SuiteName) \
namespace SuiteName \
{ \
  inline const char* GetSuiteName() { return #SuiteName; } \
} \
namespace SuiteName

/** @} **/

#endif // MACROS_H