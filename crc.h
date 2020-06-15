
#ifndef _CRC_HPP_
#include <stdint.h>
#include <vector>
uint16_t do_crc(unsigned char *ptr,int len); // 字节查表法求CRC
uint16_t do_crc_1(std::vector<char> &v, int len);
#endif //CRC
