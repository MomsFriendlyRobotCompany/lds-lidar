#include "lds_driver.h"
#include <chrono>   // milliseconds
#include <thread>   // sleep_for
#include <iostream> // printing

using namespace std;
using namespace boost;

inline void msleep(int msec){ std::this_thread::sleep_for(std::chrono::milliseconds(msec)); }

namespace lds
{
// LDS01::LDS01(const std::string& port, uint32_t baud_rate, boost::asio::io_service& io)
LDS01::LDS01(boost::asio::io_service& io): shutting_down_(false), serial_(io, "/dev/ttyUSB0"), serial(io) {
    // serial_(io);
    cout << ">> Laser: " << shutting_down_ << " " << endl;
    // serial_.set_option(boost::asio::serial_port_base::baud_rate(baud_rate_));
    // serial_.set_option(asio::serial_port_base::baud_rate(baud_rate_));

    // Below command is not required after firmware upgrade (2017.10)
    // boost::asio::write(serial_, boost::asio::buffer("b", 1));  // start motor
    // asio::write(serial_, asio::buffer("b", 1));  // start motor
}

LDS01::~LDS01(){
    // boost::asio::write(serial_, boost::asio::buffer("e", 1));  // stop motor
    // asio::write(serial_, asio::buffer("e", 1));  // stop motor
    // motor(false);
    // msleep(100);
    close();

}

bool LDS01::open(const std::string& port, uint32_t baud_rate){
    // serial_.open(port);
    serial_.set_option(asio::serial_port_base::baud_rate(baud_rate));

    if (serial_.is_open()) return true;
    return false;
}

void LDS01::close(){
    shutting_down_ = true;
    motor(false);
    msleep(100);
    if (serial_.is_open()) serial_.close();
}

void LDS01::motor(bool val){
    if (val) asio::write(serial_, asio::buffer("b", 1));  // start motor
    else asio::write(serial_, asio::buffer("e", 1));  // stop motor
    msleep(250);
}

void LDS01::poll(){
    uint8_t temp_char;
    uint8_t start_count = 0;
    bool got_scan = false;
    boost::array<uint8_t, 2520> raw_bytes;
    // array<uint8_t, 2520> raw_bytes;
    uint8_t good_sets = 0;
    uint32_t motor_speed = 0;
    rpms=0;
    int index;

    while (!shutting_down_ && !got_scan)
    {
        // Wait until first data sync of frame: 0xFA, 0xA0
        // boost::asio::read(serial_, boost::asio::buffer(&raw_bytes[start_count],1));
        asio::read(serial_, asio::buffer(&raw_bytes[start_count],1));

        if(start_count == 0){
            if(raw_bytes[start_count] == 0xFA) start_count = 1;
        }
        else if(start_count == 1){
            if(raw_bytes[start_count] == 0xA0)
            {
                start_count = 0;

                // Now that entire start sequence has been found, read in the rest of the message
                got_scan = true;

                // boost::asio::read(serial_,boost::asio::buffer(&raw_bytes[2], 2518));
                asio::read(serial_, asio::buffer(&raw_bytes[2], 2518));

                // scan->angle_min = 0.0;
                // scan->angle_max = 2.0*M_PI;
                // scan->angle_increment = (2.0*M_PI/360.0);
                // scan->range_min = 0.12;
                // scan->range_max = 3.5;
                // scan->ranges.resize(360);
                // scan->intensities.resize(360);

                //read data in sets of 6
                for(uint16_t i = 0; i < raw_bytes.size(); i=i+42)
                {
                  if(raw_bytes[i] == 0xFA && raw_bytes[i+1] == (0xA0 + i / 42)) //&& CRC check
                  {
                    good_sets++;
                    motor_speed += (raw_bytes[i+3] << 8) + raw_bytes[i+2]; //accumulate count for avg. time increment
                    rpms=(raw_bytes[i+3]<<8|raw_bytes[i+2])/10;

                    for(uint16_t j = i+4; j < i+40; j=j+6)
                    {
                      index = 6*(i/42) + (j-4-i)/6;

                      // Four bytes per reading
                      uint8_t byte0 = raw_bytes[j];
                      uint8_t byte1 = raw_bytes[j+1];
                      uint8_t byte2 = raw_bytes[j+2];
                      uint8_t byte3 = raw_bytes[j+3];

                      // Remaining bits are the range in mm
                      uint16_t intensity = (byte1 << 8) + byte0;
                      uint16_t range = (byte3 << 8) + byte2;

                      // scan->ranges[359-index] = range / 1000.0;
                      // scan->intensities[359-index] = intensity;

                      // FIXME: this should be filling an array
                      printf ("r[%d]=%f,",359-index, range / 1000.0);
                    }
                }
            }

            // scan->time_increment = motor_speed/good_sets/1e8;
          }
          else start_count = 0;
        }
    }
}
}
