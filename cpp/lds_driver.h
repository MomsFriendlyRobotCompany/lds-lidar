#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
// #include <asio.hpp>
// #include <array>

// using namespace boost;

namespace lds
{
class LDS01
{
public:
    uint16_t rpms; ///< @brief RPMS derived from the rpm bytes in an LFCD packet

    LDS01(boost::asio::io_service& io);
    ~LDS01();
    void close();
    bool open(const std::string& port, uint32_t baud_rate);
    void motor(bool value);

    void poll();


protected:
    // std::string port_; ///< @brief The serial port the driver is attached to
    // uint32_t baud_rate_; ///< @brief The baud rate for the serial connection
    bool shutting_down_; ///< @brief Flag for whether the driver is supposed to be shutting down or not
    boost::asio::serial_port serial_; ///< @brief Actual serial port object for reading/writing to the LFCD Laser Scanner
    // asio::serial_port serial_; ///< @brief Actual serial port object for reading/writing to the LFCD Laser Scanner
    uint16_t motor_speed_; ///< @brief current motor speed as reported by the LFCD.
    // boost::asio::io_service io;
};
}
