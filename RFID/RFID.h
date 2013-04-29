#ifndef RFID_H
#define RFID_H

#include "Arduino.h"
#include <HardwareSerial.h>

#define RFID_PACKET_LEN         14      // The number of bytes in a RFID code.
#define RFID_DATA_HEADER_LEN    2       // Number of trash chars in data of packet.
#define RFID_DATA_ASCII_LEN     8       // Number of ascii cahracters in RFID packet.
#define RFID_CHECKSUM_LEN       2       // Number of bytes in checksum of RFID packet.
#define RFID_START_FLAG         0x02    // Value of the start flag of RFID packet.
#define RFID_END_FLAG           0x03    // Value of the end flag of RFID packet.

class RFID {

    public:
        RFID(HardwareSerial *serial_port);
        unsigned long get_code();
        bool find_available_packet();
        void flush_buffer();
        
    private:
        HardwareSerial *_hard_serial;   // Serial port to read RFID tag code from.
        bool _is_soft_serial;           // Whether   a software serial port is being used.
        
};

#endif // RFID_H