#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "SerialPort.hpp"


namespace lds {
class LDS01
{
public:
    uint16_t rpms;
    uint32_t motor_speed;

    LDS01();
    ~LDS01();
    void close();
    bool open(const std::string& port, uint32_t baud_rate=230400);
    void motor(bool value);

    bool read();
    uint16_t scan[360]; // range in mm

protected:
    boost::asio::io_service io;
    SerialPort serial;
    uint8_t buffer[2520];  // temp buffer for
};

} // end lds
