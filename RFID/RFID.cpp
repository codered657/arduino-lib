/*  Electronic Brick RFID Reader
 *  
 *  This files contains the library for accessing the Electronic Brick RFID
 *  Reader via the Arduino serial interface.
 *
 *  Revision History:
 *      04/27/13    Steven Okai     Initial revision.
 *      04/29/13    Kevin Lo        Added functionality to search for start of packet.
 *
 */
 
#include "Arduino.h"
#include "RFID.h"
#include <HardwareSerial.h>

/*  RFID
 *  
 *  Description:    This function is the constructor for the RFID class.
 *
 *  Arguments:      serial_port - The serial port used for reading RFID tag codes.
 *  Return Value:   Instance of RFID class.
 *
 *  Input:          None.
 *  Output:         None.
 *
 *  Notes:          None.
 *
 *  Last Modified:  04/27/13
 *
 */
RFID::RFID(HardwareSerial *serial_port) {
    _hard_serial = serial_port;
    _is_soft_serial = false;
}

/*  get_code
 *  
 *  Description:    This function returns the 32-bit code for a read RFID tag.
 *
 *  Arguments:      RFID - RFID device to read from.
 *  Return Value:   32-bit RFID code (unsigned long)
 *
 *  Input:          None.
 *  Output:         None.
 *
 *  Notes:          This function assumes that there is a valid RFID tag code
 *                  in the rx buffer for the serial port used 
 *                  (find_available_packet returned true).
 *
 *  Last Modified:  04/27/13
 *
 */
unsigned long RFID::get_code() {

    char code_ascii[RFID_DATA_ASCII_LEN + 1];
    unsigned long code;
    char dummy_buf[RFID_PACKET_LEN];
    
    // Read out start byte and trash characters (RFID_DATA_HEADER_LEN)
    (*_hard_serial).readBytes(dummy_buf, 1 + RFID_DATA_HEADER_LEN);
    
    // Read out data and store for conversion to 32-bit value (unsigned long)
    (*_hard_serial).readBytes(code_ascii, RFID_DATA_ASCII_LEN);
    code_ascii[RFID_DATA_ASCII_LEN] = 0;
    
    // Convert string
    code = strtoul(code_ascii, NULL, 16);
    
    // Read out checksum and end flag
    (*_hard_serial).readBytes(dummy_buf, RFID_CHECKSUM_LEN + 1);
    
    return code;
}

/*  find_available_packet
 *  
 *  Description:    This function returns true if there is at least 1 RFID tag
 *                  code available, and false otherwise.  If the first byte in the rx
 *                  buffer is not the start flag, it will remove it.
 *
 *  Arguments:      RFID - RFID device to read from.
 *  Return Value:   Whether there is an RFID tag code available (bool)
 *
 *  Input:          Serial port (via HardwareSerial library).
 *  Output:         None.
 *
 *  Notes:          None.
 *
 *  Last Modified:  04/29/13
 *
 */
bool RFID::find_available_packet() {
        
    // If we haven't found the start of an RFID packet, discard the byte + return false.
    if ((*_hard_serial).peek() != RFID_START_FLAG) {
        Serial.read();
        return false;
    }
        
    // Check if there is at least a full RFID packet in the rx buffer.
    if ((*_hard_serial).available() >= RFID_PACKET_LEN) {
        return true;
    }
    
    return false;
}

void RFID::flush_buffer() {
    // Flush the buffer
}