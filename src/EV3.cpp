#include "EV3.h";


EV3::EV3() {

}


void EV3::sendMailbox(SoftwareSerial bt, String messageName, String message) {
  char messageNameArray[256];
  messageName.toCharArray(messageNameArray, 256);
  
  char messageArray[512];
  message.toCharArray(messageArray, 512);

  m.message.s = messageArray;
  m.type = 1;

  createPacket(packet, &packetSize, messageNameArray, m);
  printArray(packet, packetSize);
  for (int i = 0; i < packetSize; i++) {
    bt.write(packet[i]);
  }
}

void EV3::sendMailbox(SoftwareSerial bt, String messageName, float message) {
  char messageNameArray[256];
  messageName.toCharArray(messageNameArray, 256);

  m.message.f = message;
  m.type = 2;

  createPacket(packet, &packetSize, messageNameArray, m);

  for (int i = 0; i < packetSize; i++) {
    bt.write(packet[i]);
  }
}

void EV3::sendMailbox(SoftwareSerial bt, String messageName, bool message) {
  char messageNameArray[256];
  messageName.toCharArray(messageNameArray, 256);
  
  m.message.b = message;
  m.type = 3;

  createPacket(packet, &packetSize, messageNameArray, m);

  for (int i = 0; i < packetSize; i++) {
    bt.write(packet[i]);
  }
}

void EV3::createPacket(unsigned char* packet, size_t* packetSize, char* messageName, MESSAGE_STRUCT message) {
  unsigned char headers[6];
  unsigned char payload[256];
  short payloadSize;

  if (message.type == 1) {
    createPayload(payload, &payloadSize, strlen(messageName) + 1, messageName, strlen(message.message.s) + 1, message.message.s);
    createHeaders(headers, 1, SYSTEM_COMMAND_NO_REPLY, WRITEMAILBOX, payloadSize);
  } else if (message.type == 2) {
    createPayload(payload, &payloadSize, strlen(messageName) + 1, messageName, message.message.f);
    createHeaders(headers, 1, SYSTEM_COMMAND_NO_REPLY, WRITEMAILBOX, payloadSize);
  } else {
    createPayload(payload, &payloadSize, strlen(messageName) + 1, messageName, message.message.b);
    createHeaders(headers, 1, SYSTEM_COMMAND_NO_REPLY, WRITEMAILBOX, payloadSize);
  }

  *packetSize = payloadSize + 6;
  unsigned char tempPacket[*packetSize];
  mergeArrays(headers, sizeof(headers), payload, sizeof(payload), tempPacket);
  //printArray(tempPacket, *packetSize);
  for (int i = 0; i < *packetSize; i++) {
    packet[i] = tempPacket[i];
  }
}

void EV3::createHeaders(unsigned char* headers, short counter, byte commandType, byte systemCommand, short payloadSize) {
  headers[0] = payloadSize + 4 & 0xFF;
  headers[1] = (payloadSize + 4 >> 8) & 0xFF;
  headers[2] = counter & 0xFF;
  headers[3] = (counter >> 8) & 0xFF;
  headers[4] = commandType;
  headers[5] = systemCommand;

  /*Serial.println("****HEADER INFO****");
    Serial.println("Payload size: " + String(payloadSize + 4));
    Serial.print("headers: ");
    printArray(headers, 6);
    Serial.println("******************");
    Serial.println();*/
}

void EV3::createPayload(unsigned char* payload, short* payloadSize, byte nameLength, char* messageName, short messageLength, char* message) {
  payload[0] = nameLength;
  for (int i = 0; i < nameLength - 1; i++) {
    payload[1 + i] = messageName[i];
  }
  payload[1 + nameLength - 1] = '\0';
  payload[1 + nameLength] = messageLength & 0xFF;
  payload[1 + nameLength + 1] = (messageLength >> 8) & 0xFF;
  for (int i = 0; i < messageLength - 1; i++) {
    payload[1 + nameLength + 2 + i] = message[i];
  }
  payload[1 + nameLength + 2 + messageLength - 1] = 0;
  *payloadSize = 3 + nameLength + messageLength;
}

void EV3::createPayload(unsigned char* payload, short* payloadSize, byte nameLength, char* messageName, float message) {
  typedef union FLOAT_t {
    float number;
    uint8_t bytes[4];
  };
  FLOAT_t tempMessage;
  tempMessage.number = message;

  payload[0] = nameLength;
  for (int i = 0; i < nameLength - 1; i++) {
    payload[1 + i] = messageName[i];
  }
  payload[nameLength] = '\0';
  payload[1 + nameLength] = 4 & 0xFF;
  payload[2 + nameLength] = (4 >> 8) & 0xFF;
  for (int i = 0; i < 4; i++) {
    payload[3 + nameLength + i] = tempMessage.bytes[i];
  }
  *payloadSize = 3 + nameLength + 4;
  //printArray(payload, *payloadSize);
}

void EV3::createPayload(unsigned char* payload, short* payloadSize, byte nameLength, char* messageName, bool message) {
  payload[0] = nameLength;
  for (int i = 0; i < nameLength - 1; i++) {
    payload[1 + i] = messageName[i];
  }
  payload[nameLength] = '\0';
  payload[1 + nameLength] = 1 & 0xFF;
  payload[2 + nameLength] = (1 >> 8) & 0xFF;
  if (message) payload[3 + nameLength] = 1;
  else payload[3 + nameLength] = 0;
  *payloadSize = 3 + nameLength + 1;
  //printArray(payload, *payloadSize);
}

void EV3::printArray(unsigned char a[], size_t size) {
  for (int i = 0; i < size; i++) {
    Serial.print((byte)a[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void EV3::mergeArrays(unsigned char* a, size_t aSize, unsigned char* b, size_t bSize, unsigned char* merged) {
  for (int i = 0; i < aSize; i++) {
    merged[i] = a[i];
  }
  for (int i = 0; i < bSize; i++) {
    merged[aSize + i] = b[i];
  }
  //printArray(merged, aSize+bSize);
}
