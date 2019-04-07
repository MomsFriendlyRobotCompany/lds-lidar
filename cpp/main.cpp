
#include "lds_driver.h"

int main(int argc, char **argv)
{
  std::string port;
  int baud_rate;
  uint16_t rpms;
  port = "/dev/ttyUSB0";
  baud_rate = 230400;
  boost::asio::io_service io;

  try
  {
    lds::LFCDLaser laser(port, baud_rate, io);

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
