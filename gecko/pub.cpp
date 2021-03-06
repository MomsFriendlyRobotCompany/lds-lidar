// Example with Gecko
// make sure to run a pygeckocore.py instance with key: dalek.
// MIT License Kevin J Walchko 2019
//

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <gecko/gecko.hpp>
#include <gecko/msgpack/msgs.hpp>
#include <gecko/msgpack/msgpack_pub_sub.hpp>
#include "lds/lds.hpp"

using namespace std;
using namespace lds;


int main(){
    printf("hello\n");
    gecko::init();

    // string port = "/dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0";
    string port = "/dev/ttyUSB0";

    LDS01 laser;
    laser.open(port);
    laser.motor(true);
    // laser.setSerialBaudrate(baudrate);
    // laser.setIntensities(intensities);//intensity supported by S4B lidar
    // laser.setAutoReconnect(true);//hot plug
    // laser.setMaxRange(10.0);
    // laser.setMinRange(0.12);
    // laser.setMaxAngle(180);
    // laser.setMinAngle(-180);
    // laser.setScanFrequency(7);  // scanning frequency
    // laser.setSampleRate(5);  // range frequency
    // laser.setReversion(true);
    // laser.setFixedResolution(false);
    // laser.initialize();

    // Publisher *p = gecko::pubBindTCP("dalek", "lidar");
    Publisher *p = gecko::pubBindUDS("dalek", "lidar", "/tmp/test-0");
    if (p == nullptr) {
        printf("*** invalid publisher ***\n");
        return 1;
    }

    mpPublisher<lidar_st> pub(p);
    Rate rate(5);
    unsigned long long start = 0;

    while(gecko::ok()){

        if(laser.read()){
            lidar_st msg;
            double angle = 0.0;

            for (const auto r: laser.scan){
                pt_t pt(angle, r);
                msg.data.push_back(pt);

                angle += 1.0;
            }

            printf(">> lidar_st: %f\n", msg.timestamp);

            pub.publish(msg);
        }
        else {
            printf("Failed to get lidar data\n");
        }

        rate.sleep();

    }

    // laser.turnOff();
    // laser.disconnecting();

    return 0;
}
