#include <arduino.h>

#include "Dummy.h"
#include "SerialNotifier.h"

/* Must be defined in order to satisfy linker. */
extern "C" void   atexit( void ) { } 

/* This is a simple test suite */
SUITE(Suite0)
{
	TEST(Test0)
	{
		int a = 1;
		CHECK(1, a);
		CHECK("bar", "foo");
	}

	TEST(Test1)
	{
		CHECK(0, 1);
        CHECK(5, 2);
	}    
}

void setup()
{
	Serial.begin(9600);
	
	Dummy::SerialNotifier notifier;
	Dummy::Result res;
	
	res = Dummy::Runner::Instance().Run(&notifier); 
}

void loop()
{
}
