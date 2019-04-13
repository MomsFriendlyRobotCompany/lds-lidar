#include "SerialPort.hpp"
#include <sys/ioctl.h>  // dtr or rts
#include <iostream>

using namespace std;
using namespace boost::asio;
using namespace boost;
// https://www.boost.org/doc/libs/1_65_0/doc/html/boost_asio/reference/serial_port.html
// https://www.boost.org/doc/libs/1_65_0/doc/html/boost_asio/reference/basic_serial_port.html
// http://www.ridgesolutions.ie/index.php/2012/12/13/boost-c-read-from-serial-port-with-timeout-example/

SerialPort::SerialPort(boost::asio::io_service& io): serial(io){}

SerialPort::~SerialPort(){
    close();
}

void SerialPort::close(){
    if (serial.is_open()) serial.close();
}

bool SerialPort::open(const string& port, uint32_t baud_rate){
    try {
        serial.open(port);
        serial.set_option(serial_port_base::baud_rate(baud_rate));
        serial.set_option(serial_port::parity(serial_port::parity::none));
        serial.set_option(serial_port::character_size(serial_port::character_size(8)));
        serial.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
        serial.set_option(serial_port::flow_control(serial_port::flow_control::none));
    }
    catch (std::exception const& e){
        cout << "*** SerialPort::open(): " << e.what() << endl;
        return false;
    }
    return true;
}

int SerialPort::write(const string& s){
    return asio::write(serial, asio::buffer(s.c_str(),s.size()));
}

// int SerialPort::write(const std::string& s){}
size_t SerialPort::read(const size_t number){
    // uint8_t b[number];
    size_t len = asio::read(serial, asio::buffer(&buffer, number));
    return len;
}

size_t SerialPort::read(const size_t number, uint8_t *buff){
    // uint8_t b[number];
    size_t len = asio::read(serial, asio::buffer(buff, number));
    return len;
}

// size_t SerialPort::read(const size_t number){
//     // uint8_t b[number];
//     size_t len = asio::read(serial, asio::buffer(&buffer, number));
//     return len;
// }

// int SerialPort::read(string result){
//     char c;
//     // string result;
//     // int num = 0;
//     // for(;;){
//     asio::read(serial, asio::buffer(&c,1));
//     //     switch(c){
//     //         case '\r':
//     //             break;
//     //         case '\n':
//     //             return result.size();
//     //         default:
//     //             result += c;
//     //     }
//     // }
//     return result.size();
// }

// void SerialPort::dtr(bool val){
//     // serial.EnableDTR(val);
// }

void SerialPort::setRTS(bool enabled){
    int fd = serial.native_handle();
    int data = TIOCM_RTS;
    if (!enabled)
        ioctl(fd, TIOCMBIC, &data);
    else
        ioctl(fd, TIOCMBIS, &data);
}

void SerialPort::setDTR(bool enabled){
    int fd = serial.native_handle();
    int data = TIOCM_DTR;
    if (!enabled)
        ioctl(fd, TIOCMBIC, &data);        // Clears the DTR pin
    else
        ioctl(fd, TIOCMBIS, &data);        // Sets the DTR pin
}
