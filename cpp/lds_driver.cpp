#include "lds_driver.h"
#include <chrono>   // milliseconds
#include <thread>   // sleep_for
#include <iostream> // printing
#include <bits/stdc++.h>  // memset

using namespace std;
using namespace boost;

inline void msleep(int msec){ std::this_thread::sleep_for(std::chrono::milliseconds(msec)); }

namespace lds
{

LDS01::LDS01(): io(), serial(io) {}

LDS01::~LDS01(){
    motor(false);
    msleep(100);
    close();

}

bool LDS01::open(const std::string& port, uint32_t baud_rate){
    return serial.open(port, baud_rate);
}

void LDS01::close(){
    // shutting_down_ = true;
    motor(false);
    msleep(100);
    serial.close();
    printf(">> LDS-01 bye ...\n\n");
}

void LDS01::motor(bool val){
    if (val) serial.write("b");  // start motor
    else serial.write("e");  // stop motor
    msleep(250);
}


bool LDS01::read(){
    int index;

    memset(buffer, 0, sizeof(buffer));
    memset(scan, 0, sizeof(scan));

    std::size_t num = 0;
    uint8_t byte = 0;

    // wait for start character
    while (byte != 0xFA){
        num = serial.read(1, buffer);
        if (num == 1) byte = buffer[0];
        msleep(100);
        // printf(">> wait %u\n", byte);
    }
    // printf(">> start\n");

    // get next character
    num = serial.read(1, &buffer[1]);
    if (num == 1)
        if(buffer[1] == 0xA0){
            // printf(">> big loop\n");

            num = serial.read(2518, &buffer[2]);
            // printf(">> big loop %zu\n", num);
            if (num != 2518) return false;

            // scan->angle_min = 0.0;
            // scan->angle_max = 2.0*M_PI;
            // scan->angle_increment = (2.0*M_PI/360.0);
            // scan->range_min = 0.12;
            // scan->range_max = 3.5;
            // scan->ranges.resize(360);
            // scan->intensities.resize(360);

            // printf(">> read data buffer\n");

            //read data in sets of 6
            for(uint16_t i = 0; i < 2518; i=i+42){
                if(buffer[i] == 0xFA && buffer[i+1] == (0xA0 + i / 42)) //&& CRC check
                {
                    // printf("------------------------------------------\n");
                    // for(uint16_t x=0; x<42; x++) printf("%#04x ", buffer[i+x]);
                    // printf("\n");

                    // good_sets++;
                    motor_speed += (buffer[i+3] << 8) + buffer[i+2]; //accumulate count for avg. time increment
                    rpms = (buffer[i+3]<<8|buffer[i+2])/10;

                    for(uint16_t j = i+4; j < i+40; j=j+6)
                    {
                        index = 6*(i/42) + (j-4-i)/6;

                        // Four bytes per reading
                        uint8_t byte0 = buffer[j];
                        uint8_t byte1 = buffer[j+1];
                        uint8_t byte2 = buffer[j+2];
                        uint8_t byte3 = buffer[j+3];

                        // Remaining bits are the range in mm
                        uint16_t intensity = (byte1 << 8) + byte0;
                        uint16_t range = (byte3 << 8) + byte2;

                        // scan->ranges[359-index] = range / 1000.0;
                        // scan->intensities[359-index] = intensity;

                        // FIXME: this should be filling an array
                        // printf ("r[%d]=%.2f,",359-index, range / 1000.0);
                        scan[359 - index] = range;
                    }
                    // printf("\n");
                }
            }
    }
    return true;
}


}
