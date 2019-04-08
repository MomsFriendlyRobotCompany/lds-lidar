/*******************************************************************************
* Copyright (c) 2016, Hitachi-LG Data Storage
* Copyright (c) 2017, ROBOTIS
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice, this
*   list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

 /* Authors: SP Kong, JH Yang, Pyo */
 /* maintainer: Pyo */

#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
// #include <asio.hpp>
// #include <array>

// using namespace boost;

namespace lds
{
class LFCDLaser
{
public:
    uint16_t rpms; ///< @brief RPMS derived from the rpm bytes in an LFCD packet

    LFCDLaser(boost::asio::io_service& io);
    ~LFCDLaser();
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
