#include "lidar.hpp"

Lidar::Lidar(PinName tx, PinName rx) : speed(0), buffer(0), serial(tx, rx) {
    serial.baud(115200);
}

uint16_t Lidar::getData(uint16_t angle) {
    return this->distance[angle];
}

uint16_t Lidar::getSpeed() {
    return this->speed;
}

uint16_t Lidar::checksum() {
    uint16_t buffer[10];
    uint16_t checksum;
    uint32_t chk32 = 0;

    // Read data as 16-bit integers
    for (uint8_t i = 0; i < 10; i++) buffer[i] = this->packet[2 * i] + (this->packet[2 * i + 1] << 8);

    // Compute checksum
    for (uint8_t i = 0; i < 10; i++) chk32 = (chk32 << 1) + buffer[i];

   // Format the checksum so that it fits in 15 bits
   checksum = (chk32 & 0x7FFF) + ( chk32 >> 15 );
   
   // Truncate to 15 bits
   checksum = checksum & 0x7FFF;
   
   return checksum;
}

void Lidar::parseData() {
    uint16_t index = LIDAR_INDEX_MIN;
    uint8_t invalidFlag[4], warningFlag[4];
    uint16_t checksum, checksumCalc;

    // Calculate packet checksum
    // If the checksum is not ok, all 4 values are discarded
    checksum = (this->packet[21] << 8) + this->packet[20];
    checksumCalc = this->checksum();
    if (checksum != checksumCalc) {
        // Data not valid, abort mission !
        return;
    }

    // Retrieve which angle we are actually covering (0 -> (0 -> 3), 1 -> (4 -> 7), etc...)
    // 0xA0 corresponds to 0 and 0xF9 to 89
    index = (this->packet[1] - LIDAR_INDEX_MIN) * 4;

    // Retrieve the speed of the lidar (in little-endian)
    speed = ((this->packet[3] << 8) | this->packet[2]) / 64;
    
    // Retrieving error and warning flags (but I don't know what they mean yet)
    invalidFlag[0] = (this->packet[5]  & 0x80) >> 7;
    invalidFlag[1] = (this->packet[9]  & 0x80) >> 7;
    invalidFlag[2] = (this->packet[13] & 0x80) >> 7;
    invalidFlag[3] = (this->packet[17] & 0x80) >> 7;
    warningFlag[0] = (this->packet[5]  & 0x40) >> 6;
    warningFlag[1] = (this->packet[9]  & 0x40) >> 6;
    warningFlag[2] = (this->packet[13] & 0x40) >> 6;
    warningFlag[3] = (this->packet[17] & 0x40) >> 6;

    // If the data is has an invalid flag, it is ignored
    // Warning flags are here but not taken into consideration
    for (uint8_t i = 0; i < 4; i++) {
        if (!invalidFlag[i]) {
            this->distance[index + i] = this->packet[4 + i * 4] | ((this->packet[5 + i * 4] & 0x3F) << 8);
        }
    }
}

void Lidar::readSerial() {
    // Wait for the start byte
    if (this->serial.readable()) {
        this->packet[0] = this->serial.getc();
    }
    // The start byte is acquired
    // Read the 21 other bytes
    if (this->packet[0] == LIDAR_START_BYTE) {
        uint8_t i = 1;
        while (i < LIDAR_PACKET_LENGTH) {
            if (this->serial.readable()) {
                this->packet[i] = this->serial.getc();
                i++;
            }
        }
        // Check if the index is inside the valid range
        if (this->packet[1] >= LIDAR_INDEX_MIN && this->packet[1] <= LIDAR_INDEX_MAX) {
            // Data seems ok, parse it
            this->parseData();
            // Now the data is updated
        }

    }
}