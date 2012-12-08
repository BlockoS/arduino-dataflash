/**************************************************************************//**
 * @file SerialNotifier.cpp
 *
 * @par Copyright: 
 * - Copyright (C) 2012 by Vincent Cruz.
 * All rights reserved.
 *
 * @author Vincent Cruz cruz.vincent@gmail.com
 *
 * @par Description:
 *  Arduino serial text notifier.
 *
 * @par History:
 * - Version 0.1, 08 Dec 2012.
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
#include "SerialNotifier.h"

#include <arduino.h>

/**
 * @addtogroup DummyUnitTest
 * @{
 **/
namespace Dummy
{
	static const char* expectedPrefix = " expected=";
	static size_t expectedPrefixLen   = 10;
	
	static const char* valuePrefix = " value=";
	static size_t valuePrefixLen   = 7;
	
	/**
 	 * Constructor.
 	 */
	SerialNotifier::SerialNotifier(size_t maxValueLen, size_t maxFilenameLen, size_t maxSuitenameLen, size_t maxTestnameLen)
		: m_allowedValueLen(maxValueLen)
		, m_allowedFilenameLen(maxFilenameLen)
		, m_allowedSuitenameLen(maxSuitenameLen)
		, m_allowedTestnameLen(maxTestnameLen)
	{}
	/** 
	 * Destructor.
	 */
	SerialNotifier::~SerialNotifier()
	{}
	/**
	 * Add current string to notification string and cut it if necessary.
	 * @param [out] out Notification string pointer.
	 * @param [in] in Input string.
	 * @param [in] len Input string length.
	 * @param [in] allowedLen Allowed output length.
	 **/
	void SerialNotifier::FormatString(char* &out, const char* in, size_t len, size_t allowedLen)
	{
		size_t i;
		bool cutString = (len > allowedLen);
		if(cutString)
		{
			char* dest = out + allowedLen;
			out = dest;
			in += len;
			for(i=allowedLen; i>=3; i--)
			{
				*dest-- = *in--;
			}
			for(i=0; i<3; i++)
			{
				*dest-- = '.';
			}
		}
		else
		{
			for(i=0; i<len; i++)
			{
				*out++ = *in++;
			}
		}
	}
	/**
	 * Build failure notification string and send it via Serial.
	 * @param [in] expected Expected value.
	 * @param [in] value Obtained value.
	 * @param [in] infos Test infos.
	 */
	void SerialNotifier::NotifyFailure(char const* expected, char const* value, Dummy::Infos const& infos)
	{
		// Build string
		const char* filename  = infos.Filename();
		const char* suitename = infos.SuiteName();
		const char* testname  = infos.TestName();
		
		size_t filenameLen  = strlen(filename);
		size_t suitenameLen = strlen(suitename);
		size_t testnameLen  = strlen(testname);
		size_t expectedLen  = strlen(expected);
		size_t valueLen     = strlen(value);

		char* out = m_notificationString;
		FormatString(out, filename, filenameLen, DefaultFilenameLen);
		
		out += snprintf(out, out-m_notificationString, ":%d ", infos.Line());
		
		FormatString(out, suitename, suitenameLen, DefaultSuitenameLen);
		*out++ = ':';
		FormatString(out, testname, testnameLen, DefaultTestnameLen);

		memcpy(out, expectedPrefix, expectedPrefixLen);
		out += expectedPrefixLen;
		FormatString(out, expected, expectedLen, DefaultValueLen);
		
		memcpy(out, valuePrefix, valuePrefixLen);
		out += valuePrefixLen;
		FormatString(out, value,    valueLen,    DefaultValueLen);
		
		*out = '\0';
		
		// Send it to serial.
		Serial.println(m_notificationString);
	}
	
	/**
	 * Tests result notification and send it via Serial. 
	 * @param [in] result : tests result.
	 **/
    void SerialNotifier::NotifyResult(const Result& result)
    {
		// Build string.
		snprintf(m_notificationString, MAX_NOTIFICATION_STRING_LEN, "Test run: %d, failed: %d, errors: %d", result.total, result.failed, result.error);
		
		// Send it to serial.
		Serial.println(m_notificationString);
    }
    
};
/** @} **/