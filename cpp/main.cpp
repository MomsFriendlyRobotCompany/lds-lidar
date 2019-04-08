
#include "lds_driver.h"

int main(int argc, char **argv)
{
  std::string port = "/dev/ttyUSB0";
  int baud_rate = 230400;
  uint16_t rpms;
  // port = "/dev/ttyUSB0";
  // baud_rate = 230400;
  boost::asio::io_service io;

  try
  {

    // return 0;

    lds::LFCDLaser laser(io);

    // return 0;

    laser.open(port, baud_rate);
    laser.motor(true);

    while (1)
    {
      laser.poll();
    }
    laser.close();

    return 0;
  }
  catch (boost::system::system_error ex)
  {
    printf("An exception was thrown: %s", ex.what());
    return -1;
  }
}
