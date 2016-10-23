#ifndef LIDAR_HPP
#define LIDAR_HPP

#define LIDAR_PACKET_LENGTH 22
#define LIDAR_MAX_ANGLES    360
#define LIDAR_START_BYTE    0xFA
#define LIDAR_INDEX_MIN     0xA0
#define LIDAR_INDEX_MAX     0xF9

#include "mbed.h"

class Lidar {
public:
    Lidar(PinName tx, PinName rx);
    uint16_t getData(uint16_t angle);
    uint16_t getSpeed();
    void readSerial();

private:
    uint16_t packet[LIDAR_PACKET_LENGTH];
    uint16_t distance[LIDAR_MAX_ANGLES];
    
    uint16_t speed;
    uint8_t buffer;
    
    uint16_t checksum();
    void parseData();
    
    Serial serial;
};

#endif