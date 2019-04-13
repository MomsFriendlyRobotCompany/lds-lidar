#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <array>
#include <boost/asio.hpp>


/**
 * Creates an 8N1 serial port throws boost::system::system_error if
 * there is an error
 *
 * I shouldn't need to link aginst boost because C++11 includes asio (right?)
 * but for some reason I have too.
 */
class SerialPort
{
public:
    SerialPort(boost::asio::io_service& io);
    ~SerialPort();

    void close();
    bool open(const std::string& port, uint32_t baud_rate);

    // int write(char *s, uint16_t num){}
    int write(const std::string& s);
    int write(const std::vector<uint8_t>& buffer){}
    // void writeString(const std::string& s);

    // std::array<std::uint8_t,1024> read(const size_t number=1024);
    size_t read(const size_t number);
    size_t read(const size_t number, uint8_t *buffer);

    // void dtr(bool val);
    void setRTS(bool enabled);
    void setDTR(bool enabled);

// protected:
    // boost::asio::io_service io;
    boost::asio::serial_port serial;
    std::array<std::uint8_t,8*1024> buffer;
};
