#include "Dummy.h"

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

class SimpleFixture
{
    public:
        void Setup()
        {
            fprintf(stdout, "Fixture setup!\n");
            m_fixtureData = 2;
        }
        
        void TearDown()
        {
            fprintf(stdout, "Fixture teardown!\n");
        }
    
    protected:
        int m_fixtureData;
};

SUITE(Suite1)
{
    TEST_FIXTURE(Test2, SimpleFixture)
    {
        fprintf(stdout, "Test with fixture.\n");
        CHECK(2, m_fixtureData);
        CHECK(0, m_fixtureData);
    }
    
    TEST(Ok)
    {
        CHECK(0, 0);
    }
    
    TEST(Empty)
    {}
}

TEST(LonelyTest)
{
    const char* str = "ni!";    
    CHECK("ni!", str);
    CHECK("boing!", str);
}

/* Error notifier using the CHECK failure notification interface. */
class SimpleFailureNotifier : public Dummy::CheckFailCallbackInterface
{
    public:
        virtual ~SimpleFailureNotifier() {}
        virtual void Notify(char const* expected, char const* value, Dummy::Infos const& infos)
        {
            fprintf(stderr, "(simple notifier) %s: %s::%s failed at line %d (expected %s, value %s).\n"
                          , infos.Filename(), infos.SuiteName(), infos.TestName(), infos.Line(), expected, value);
        }
};

/* This notifier will be used with the CHECK failure notification template. */
class Foo
{
    public:
        void Bar(char const* expected, char const* value, Dummy::Infos const& infos)
        {
            fprintf(stdout, "(Foobar) %s+%d: %s::%s has failed with value %s (expected %s).\n"
                          , infos.Filename(), infos.Line(), infos.SuiteName(), infos.TestName(), value, expected);
        }
};

int main()
{
    Dummy::Result res;
    
    SimpleFailureNotifier simpleNotifier;
    
    Foo snafu;
    Dummy::CheckFailCallback<Foo> complicatedNotifier(&snafu, &Foo::Bar);
    
    /*
     * There are 6 tests. Only two are succesful. And among the 9 checks, 5 failed.
     */
    
	res = Dummy::Runner::Instance().Run(&simpleNotifier); 
    fprintf(stdout, "\nFirst run:\n\ttests run %d\n\ttest failed %d\n\tcheck failed %d\n\n", res.total, res.failed, res.error);
    
    res = Dummy::Runner::Instance().Run(&complicatedNotifier);
    fprintf(stdout, "\nSecond run:\n\ttests run %d\n\ttest failed %d\n\tcheck failed %d\n", res.total, res.failed, res.error);

    return 0;
}