
#include "lds_driver.h"
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char **argv)
{
    std::string port = "/dev/ttyUSB0";
    int baud_rate = 230400;
    uint16_t rpms;
    // port = "/dev/ttyUSB0";
    // baud_rate = 230400;
    // boost::asio::io_service io;

    try {
        lds::LDS01 laser;


        bool val = laser.open(port, baud_rate);
        cout << ">> open serial: " << val << endl;
        laser.motor(false);

        sleep(2);
        laser.motor(true);

        int count = 5;

        while (count--) {
            laser.read();

            printf("-----------------------\n");
            for (const auto r: laser.scan) printf("%.2f ", r / 1000.0);
            printf("\n");
        }

        // laser.close();

        // sleep(1);

        return 0;
    }
    catch (boost::system::system_error ex) {
        printf("An exception was thrown: %s", ex.what());
        return -1;
    }
}
