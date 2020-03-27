/*
  EV3.h - Library for communicating with an Lego EV3 Brick.
  Created by Gkagkas Anastasios, March 20, 2020.
  Released into the public domain.
*/

#ifndef EV3_h
#define EV3_H

#include "Arduino.h"
#include <SoftwareSerial.h>

/*
   http://ev3.fantastic.computer/doxygen/systemcommands.html
*/

/*
  System Command Bytes:
  , ------, ------, ------, ------, ------, ------, ------, ------,
  | Byte 0 | Byte 1 | Byte 2 | Byte 3 | Byte 4 | Byte 5 |      | Byte n |
  '------'------'------'------'------'------'------'------'

  Byte 0 – 1: Command size, Little Endian\n

  Byte 2 – 3: Message counter, Little Endian\n

  Byte 4:     Command type. see following defines   */

#define     SYSTEM_COMMAND_REPLY          0x01    //  System command, reply required
#define     SYSTEM_COMMAND_NO_REPLY       0x81    //  System command, reply not required
/*
  Byte 5:     System Command. see following defines */

#define     BEGIN_DOWNLOAD                0x92    //  Begin file down load
#define     CONTINUE_DOWNLOAD             0x93    //  Continue file down load
#define     BEGIN_UPLOAD                  0x94    //  Begin file upload
#define     CONTINUE_UPLOAD               0x95    //  Continue file upload
#define     BEGIN_GETFILE                 0x96    //  Begin get bytes from a file (while writing to the file)
#define     CONTINUE_GETFILE              0x97    //  Continue get byte from a file (while writing to the file)
#define     CLOSE_FILEHANDLE              0x98    //  Close file handle
#define     LIST_FILES                    0x99    //  List files
#define     CONTINUE_LIST_FILES           0x9A    //  Continue list files
#define     CREATE_DIR                    0x9B    //  Create directory
#define     DELETE_FILE                   0x9C    //  Delete
#define     LIST_OPEN_HANDLES             0x9D    //  List handles
#define     WRITEMAILBOX                  0x9E    //  Write to mailbox
#define     BLUETOOTHPIN                  0x9F    //  Transfer trusted pin code to brick
#define     ENTERFWUPDATE                 0xA0    //  Restart the brick in Firmware update mode
#define     SETBUNDLEID                   0xA1    //  Set Bundle ID for mode 2
#define     SETBUNDLESEEDID               0xA2    //  Set bundle seed ID for mode 2

/*

  Byte 6 - n: Dependent on System Command

*/
/*
  System Command Response Bytes:
  ,------,------,------,------,------,------,------,------,
  |Byte 0|Byte 1|Byte 2|Byte 3|      |      |      |Byte n|
  '------'------'------'------'------'------'------'------'

  Byte 0 – 1: Reply size, Little Endian\n

  Byte 2 – 3: Message counter, Little Endian\n

  Byte 4:     Reply type. see following defines     */

#define     SYSTEM_REPLY                  0x03    //  System command reply
#define     SYSTEM_REPLY_ERROR            0x05    //  System command reply error
/*
  Byte 5:     System command this is the response to

  Byte 6:     Reply status
*/

// SYSTEM command return codes
#define     SUCCESS                       0x00
#define     UNKNOWN_HANDLE                0x01
#define     HANDLE_NOT_READY              0x02
#define     CORRUPT_FILE                  0x03
#define     NO_HANDLES_AVAILABLE          0x04
#define     NO_PERMISSION                 0x05
#define     ILLEGAL_PATH                  0x06
#define     FILE_EXITS                    0x07
#define     END_OF_FILE                   0x08
#define     SIZE_ERROR                    0x09
#define     UNKNOWN_ERROR                 0x0A
#define     ILLEGAL_FILENAME              0x0B
#define     ILLEGAL_CONNECTION            0x0C

/*
  Byte 7 - n: Response dependent on System Command
*/

class EV3 {
  public:
    EV3();
    void sendMailbox(SoftwareSerial bt, String messageName, String message);
    void sendMailbox(SoftwareSerial bt, String messageName, float message);
    void sendMailbox(SoftwareSerial bt, String messageName, bool message);
    void printArray(unsigned char a[], size_t size);
  private:
    unsigned char packet[256]; //The packet sent
    size_t packetSize;         //The packet size
    typedef union MESSAGE {
      float f;
      char* s;
      bool b;
    };
    typedef struct MESSAGE_STRUCT {
      MESSAGE message;
      short type;
    };
    MESSAGE_STRUCT m;
    
    void createPacket(unsigned char* packet, size_t* packetSize, char* messageName, MESSAGE_STRUCT message);
    void createHeaders(unsigned char* headers, short counter, byte commandType, byte systemCommand, short payloadSize);
    void createPayload(unsigned char* payload, short* payloadSize, byte nameLength, char* messageName, short messageLength, char* message);
    void createPayload(unsigned char* payload, short* payloadSize, byte nameLength, char* messageName, float message);
    void createPayload(unsigned char* payload, short* payloadSize, byte nameLength, char* messageName, bool message);
    void mergeArrays(unsigned char* a, size_t aSize, unsigned char* b, size_t bSize, unsigned char* merged);
};

#endif
