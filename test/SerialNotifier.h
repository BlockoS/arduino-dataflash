/**************************************************************************//**
 * @file SerialNotifier.h
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
#ifndef SERIAL_NOTIFIER_H
#define SERIAL_NOTIFIER_H

#include <stdlib.h>
#include "Callbacks.h"

/**
 * @addtogroup DummyUnitTest
 * @{
 **/
namespace Dummy
{
	#define MAX_NOTIFICATION_STRING_LEN 128
	
	/**
	 * Display test failure informations on the serial line.
	 * The notification is : "filename:line suite test expected_value test_value"
	 * If filename, suite, test or values are too long they are cut and "..." is appended.
	 */
	class SerialNotifier : public Dummy::TestNotificationInterface
	{
		public:
			enum
			{
				DefaultValueLen     = 32,
				DefaultFilenameLen  = 32,
				DefaultSuitenameLen = 16,
				DefaultTestnameLen  = 16,
				MaxStringLen        = 127
			};

		public:
			SerialNotifier(size_t maxValueLen=DefaultValueLen, size_t maxFilenameLen=DefaultFilenameLen, size_t maxSuitenameLen=DefaultSuitenameLen, size_t maxTestnameLen=DefaultTestnameLen);
			virtual ~SerialNotifier();
			virtual void NotifyFailure(char const* expected, char const* value, Dummy::Infos const& infos);
			virtual void NotifyResult(const Result& result);
			
		protected:
			void FormatString(char* &out, const char* in, size_t len, size_t allowedLen);

		protected:
			size_t m_allowedValueLen;
			size_t m_allowedFilenameLen;
			size_t m_allowedSuitenameLen;
			size_t m_allowedTestnameLen;
			char m_notificationString[MAX_NOTIFICATION_STRING_LEN];
	};
};
/** @} **/

#endif // SERIAL_NOTIFIER_H