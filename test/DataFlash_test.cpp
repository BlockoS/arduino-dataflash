#include <SPI.h>
#include <DataFlash.h>

#include "arduino-test/Dummy.h"

class DataFlashFixture
{
    public:
        static const int8_t CHIP_SELECT   = 5;
        static const int8_t RESET         = 6;
        static const int8_t WRITE_PROTECT = 7;

    public:
        void Setup()
        {
            /* Initialize SPI */
            SPI.begin();

            /* Let's wait 1 second, allowing use to press the serial monitor button :p */
            delay(1000);

            /* Initialize dataflash */
            m_dataflash.setup(CHIP_SELECT, RESET, WRITE_PROTECT);

            delay(10);

            /* Read status register */
            m_status = m_dataflash.status();

            /* Read manufacturer and device ID */
            m_dataflash.readID(m_id);
        }

        void TearDown()
        {
            /* Disable dataflash */
            m_data.disable();
            /* And SPI */
            SPI.end();
        }

    protected:
        DataFlash m_dataflash;
        uint8_t m_status;
        DataFlash::ID m_id;
};

class SerialNotifier : public Dummy::CheckFailCallbackInterface
{
    public:
        SerialNotifier() {}
        virtual ~SerialNotifier() {}

        virtual void Notify(char const* expected, char const* value, Dummy::Infos const& infos)
        {
            Serial.print("%s: %s::%s failed at line %d (expected %s, value %s).\n", 
                         infos.Filename(), infos.SuiteName(), infos.TestName(), infos.Line(), expected, value);
        }
};


SUITE(SingleDeviceTest)
{
    TEST_FIXTURE(InitializationTest, DataFlashFixture)
    {
        int i;
        uint8_t validDensity[10] = 
        { 0x0c, 0x14, 0x1c, 0x24, 0x2c, 0x34, 0x3c, 0x10, 0x18, 0x20 };

        CHECK(AT45_READY, m_status & AT45_READY);
        for(i=0; i<10; i++)
        {
            CHECK(validDensity[i], m_status & 0x3f);
        }
        CHECK(0x1f, m_id.manufacture);		
    }

    TEST_FIXTURE(BufferReadWriteTest, DataFlashFixture)
    {
        // [todo]
    }
}

/* Arduino setup hook */
void setup()
{
    Serial.begin(115200);
    SerialNotifier notifier;

    res = Dummy::Runner::Instance().Run(&notifier); 
    Serial.print("tests run %d\n\ttest failed %d\n\tcheck failed %d\n", res.total, res.failed, res.error);
}

/* Arduino loop */
void loop()
{}

