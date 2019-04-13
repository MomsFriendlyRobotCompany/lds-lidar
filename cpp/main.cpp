
#include "lds/lds.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char **argv) {
    try {
        lds::LDS01 laser;
        std::string port = "/dev/ttyUSB0";
        // string port = "/dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0";

        bool val = laser.open(port);
        cout << ">> open serial: " << val << endl;
        // laser.motor(false);

        // sleep(2);
        laser.motor(true);

        int count = 5;

        while (count--) {
            laser.read();

            printf("[LDS Ranges] -----------------------\n");
            for (const auto r: laser.scan) printf("%.2f ", r / 1000.0);
            printf("\n");
        }

        return 0;
    }
    catch (boost::system::system_error ex) {
        printf("An exception was thrown: %s", ex.what());
        return -1;
    }
}
