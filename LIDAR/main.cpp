#define LIDAR_PACKET_LENGTH 22
#define LIDAR_MAX_ANGLES    360
#define LIDAR_START_BYTE    0xFA
#define LIDAR_INDEX_MIN     0xA0
#define LIDAR_INDEX_MAX     0xF9
#define NUCLEO_F411RE_TX    PA_2
#define NUCLEO_F411RE_RX    PA_3
#define NUCLEO_F411RE_BTN   PC_13
#define NUCLEO_F429ZI_TX    PD_5
#define NUCLEO_F429ZI_RX    PD_6
#define INIT_START_NOK      0
#define INIT_START_OK       1
#define INIT_ANGLE_OK       2

#include "mbed.h"

uint16_t packet[LIDAR_PACKET_LENGTH];
uint16_t distance[LIDAR_MAX_ANGLES];

uint16_t good_readings = 0;
uint16_t bad_readings = 0;
uint16_t angles_covered = 0;
uint8_t init_level = 0;
uint8_t ch = 0;
uint16_t speed = 0;

Serial pc(SERIAL_TX, SERIAL_RX);
//Serial lidar(NUCLEO_F429ZI_TX, NUCLEO_F429ZI_RX);
Serial lidar(NUCLEO_F411RE_TX, NUCLEO_F411RE_RX);
DigitalIn btn(NUCLEO_F411RE_BTN);

void clear_terminal(void) {
    pc.putc(27);
    pc.printf("[2J");
    pc.putc(27);
    pc.printf("[H");
}

uint16_t get_checksum(uint16_t * packet) {
    uint16_t data[10];
    uint16_t checksum;
    uint32_t chk32 = 0;

    // Reading the checksum data
    for (uint8_t i = 0; i < 10; i++) data[i] = packet[2 * i] + (packet[2 * i + 1] << 8);

    // Compute checksum
    for (uint8_t i = 0; i < 10; i++) chk32 = (chk32 << 1) + data[i];

   // Format the checksum so that it fits in 15 bits
   checksum = (chk32 & 0x7FFF) + ( chk32 >> 15 );
   // Truncate to 15 bits
   checksum = checksum & 0x7FFF;
   return checksum;
}

void parse_data(uint16_t *packet) {
    uint16_t index = LIDAR_INDEX_MIN;
    uint8_t invalid_flag[4], warning_flag[4];
    uint16_t checksum, checksum_calc;

    // Calculate packet checksum
    // If the checksum is not ok, all 4 values are discarded
    checksum = (packet[21] << 8) + packet[20];
    checksum_calc = get_checksum(packet);
    if (checksum != checksum_calc) {
        bad_readings += 4;
    }

    // Retrieving which angle we are actually covering (0 -> (0 -> 3), 1 -> (4 -> 7), etc...)
    // 0xA0 corresponds to 0 and 0xF9 to 89
    index = (packet[1] - LIDAR_INDEX_MIN) * 4;

    // Retrieving the speed of the lidar (in little-endian)
    speed = ((packet[3] << 8) | packet[2]) / 64;

    // Retrieving error flags (but I don't know what they mean yet)
    invalid_flag[0] = (packet[5] & 0x80) >> 7;
    invalid_flag[1] = (packet[9] & 0x80) >> 7;
    invalid_flag[2] = (packet[13] & 0x80) >> 7;
    invalid_flag[3] = (packet[17] & 0x80) >> 7;
    warning_flag[0] = (packet[5] & 0x40) >> 6;
    warning_flag[1] = (packet[9] & 0x40) >> 6;
    warning_flag[2] = (packet[13] & 0x40) >> 6;
    warning_flag[3] = (packet[17] & 0x40) >> 6;

    // If we are back to the 1st packet, we reset the error counters and the angle counter
    if (!index) {
        angles_covered = 0;
        for (uint16_t i = 0; i < 360; i++) if (distance[i] > 0) angles_covered++;
        good_readings = 0;
        bad_readings = 0;
    }

    for (uint8_t i = 0; i < 4; i++) {
        if (!invalid_flag[i]) {
            distance[index + i] = packet[4 + i * 4] | ((packet[5 + i * 4] & 0x3F) << 8);
            good_readings++;
        } else {
            distance[index + i] = 0;
            bad_readings++;
        }
    }
}
void send_speed(void) {
    pc.printf("SPD%4d\r\n", speed);
}

void send_data(uint16_t *data) {
    pc.printf("DAT");
    for (uint16_t i = 0; i < LIDAR_MAX_ANGLES; i++) {
        pc.printf("%5d", data[i]);
    }
    pc.printf("\r\n");
}

void read(void) {
    while(1) {
        // clear_terminal();
        if (lidar.readable()) packet[0] = lidar.getc();
        if (packet[0] == LIDAR_START_BYTE) {
            uint8_t i = 1;
            while (i < LIDAR_PACKET_LENGTH) {
                if (lidar.readable()) {
                    packet[i] = lidar.getc();
                    i++;
                }
            }
            if (packet[1] >= LIDAR_INDEX_MIN && packet[1] <= LIDAR_INDEX_MAX) {
                parse_data(packet);
                send_speed();
                if (angles_covered == LIDAR_MAX_ANGLES) send_data(distance);
                //pc.printf("%d\t%d\t%d\t%d\r\n", speed, good_readings, bad_readings, angles_covered);
            }

        }
    }
}

int main(void) {
    lidar.baud(115200);
    pc.baud(115200);

    //Thread t1;
    //t1.start(&read);
    //t1.join();

    speed = 312;
    uint16_t d1[360] = {1414,1414,1415,1416,1417,1419,1422,1424,1428,1431,1436,1440,1445,1451,1457,1464,1471,1478,1486,1495,1504,1514,1525,1536,1548,1560,1573,1587,1601,1616,1632,1649,1667,1686,1705,1726,1748,1770,1794,1819,1846,1873,1903,1933,1965,1999,1939,1882,1829,1780,1734,1690,1649,1611,1575,1541,1508,1478,1449,1422,1396,1371,1348,1326,1305,1285,1266,1248,1230,1214,1198,1184,1170,1156,1143,1131,1120,1109,1098,1088,1079,1070,1061,1053,1046,1039,1032,1025,1020,1014,1009,1004, 999, 995, 991, 988, 985, 982, 979, 977, 975, 973, 972, 971, 970, 970, 970, 970, 970, 971, 972, 973, 975, 977, 979, 982, 985, 988, 991, 995, 999, 970, 943, 918, 894, 871, 850, 830, 812, 794, 777, 762, 747, 733, 719, 707, 695, 683, 672, 662, 652, 643, 634, 626, 618, 610, 603, 596, 589, 583, 577, 571, 566, 561, 556, 551, 547, 543, 539, 535, 532, 528, 525, 522, 520, 517, 515, 513, 511, 509, 507, 506, 504, 503, 502, 501, 501, 500, 500, 500, 500, 500, 500, 500, 501, 501, 502, 503, 504, 506, 507, 509, 511, 513, 515, 517, 520, 522, 525, 528, 532, 535, 539, 543, 547, 551, 556, 561, 566, 571, 577, 583, 589, 596, 603, 610, 618, 626, 634, 643, 652, 662, 672, 683, 695, 707, 719, 733, 747, 762, 777, 794, 812, 830, 850, 871, 894, 918, 943, 970, 999, 995, 991, 988, 985, 982, 979, 977, 975, 973, 972, 971, 970, 970, 970, 970, 970, 971, 972, 973, 975, 977, 979, 982, 985, 988, 991, 995, 999,1004,1009,1014,1020,1025,1032,1039,1046,1053,1061,1070,1079,1088,1098,1109,1120,1131,1143,1156,1170,1184,1198,1214,1230,1248,1266,1285,1305,1326,1348,1371,1396,1422,1449,1478,1508,1541,1575,1611,1649,1690,1734,1780,1829,1882,1939,1999,1965,1933,1903,1873,1846,1819,1794,1770,1748,1726,1705,1686,1667,1649,1632,1616,1601,1587,1573,1560,1548,1536,1525,1514,1504,1495,1486,1478,1471,1464,1457,1451,1445,1440,1436,1431,1428,1424,1422,1419,1417,1416,1415,1414};
    uint16_t d2[360] = {1814,1814,1815,1816,1818,1821,1824,1827,1832,1836,1842,1848,1854,1861,1869,1878,1887,1897,1907,1918,1930,1943,1956,1970,1985,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,1960,1893,1830,1772,1719,1669,1622,1578,1537,1498,1462,1428,1396,1366,1337,1310,1284,1260,1237,1215,1195,1175,1157,1139,1122,1106,1091,1076,1062,1049,1037,1025,1013,1003, 992, 983, 973, 965, 956, 948, 941, 934, 927, 921, 915, 909, 904, 899, 894, 890, 886, 882, 879, 876, 873, 820, 718, 639, 575, 524, 480, 444, 413, 386, 362, 342, 323, 307, 292, 279, 266, 255, 245, 236, 228, 220, 213, 206, 200, 194, 188, 183, 178, 174, 170, 166, 162, 158, 155, 152, 149, 146, 143, 141, 139, 136, 134, 132, 130, 128, 126, 125, 123, 122, 120, 119, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 107, 106, 105, 105, 104, 104, 103, 103, 102, 102, 101, 101, 101, 100, 100, 100, 100, 100, 100, 100, 100,   0, 100, 100, 100, 100, 100, 100, 100, 100, 101, 101, 101, 102, 102, 103, 103, 104, 104, 105, 105, 106, 107, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 119, 120, 122, 123, 125, 126, 128, 130, 132, 134, 136, 139, 141, 143, 146, 149, 152, 155, 158, 162, 166, 170, 174, 178, 183, 188, 194, 200, 206, 213, 220, 228, 236, 245, 255, 266, 279, 292, 307, 323, 342, 362, 386, 413, 444, 480, 524, 575, 639, 718, 820, 873, 876, 879, 882, 886, 890, 894, 899, 904, 909, 915, 921, 927, 934, 941, 948, 956, 965, 973, 983, 992,1003,1013,1025,1037,1049,1062,1076,1091,1106,1122,1139,1157,1175,1195,1215,1237,1260,1284,1310,1337,1366,1396,1428,1462,1498,1537,1578,1622,1669,1719,1772,1830,1893,1960,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,1985,1970,1956,1943,1930,1918,1907,1897,1887,1878,1869,1861,1854,1848,1842,1836,1832,1827,1824,1821,1818,1816,1815,1814};
    while(1) {
        Thread::wait(1);
        send_speed();
        if (btn) send_data(d1);
        else send_data(d2);
    }
}
