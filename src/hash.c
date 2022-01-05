#include "durex.h"

static const uint32_t k[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};
 
static const uint32_t r[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

void w32_to_byte(uint32_t val, uint8_t bytes[])
{
    bytes[0] = (uint8_t) val;
    bytes[1] = (uint8_t) (val >> 8);
    bytes[2] = (uint8_t) (val >> 16);
    bytes[3] = (uint8_t) (val >> 24);
}
 
void byte_to_w32(uint8_t val[], uint32_t bytes[])
{
    *bytes = (uint32_t)val[0] | ((uint32_t)val[1] << 8) | ((uint32_t)val[2] << 16) | ((uint32_t)val[3] << 24);
}

void w32_to_4bytes(uint32_t val, uint32_t bytes[])
{
    bytes[3] = val;
    bytes[2] = (val >> 8);
    bytes[1] = (val >> 16);
    bytes[0] = (val >> 24);
}

uint32_t md5_lrot(uint32_t x, uint32_t y)
{
    return ((x << y) | (x >> (32 - y)));
}

uint8_t *_md5(uint8_t *data)
{
    uint8_t digest[16];
    uint32_t hblock[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
    uint8_t *msg = NULL;
    size_t len, offset, slen;
    uint32_t i, tblock;
    uint32_t w[16], blocks[4], pos[2];

    slen = strlen((const char*)data);
    for (len = slen + 1; len % 64 != 56; len++)
        ;
    msg = (uint8_t*)malloc(len + 8);
    memcpy(msg, data, slen);
    msg[slen] = 0x80;
    for (offset = slen + 1; offset < len; offset++)
        msg[offset] = 0;
    w32_to_byte(slen * 8, msg + len);
    w32_to_byte(slen >> 29, msg + len + 4);
    for(offset=0; offset<len; offset += 64)
    {
        for (i = 0; i < 16; i++)
            byte_to_w32(msg + offset + i * 4, &w[i]);
        blocks[0]= hblock[0];
        blocks[1]= hblock[1];
        blocks[2]= hblock[2];
        blocks[3]= hblock[3];
        for(i = 0; i < 64; i++)
        {
            pos[0] = (i < 16) ? (blocks[1]& blocks[2]) | ((~blocks[1]) & blocks[3]) : (i < 32) ? (blocks[3] & blocks[1]) | ((~blocks[3]) & blocks[2]) : (i < 48) ? (blocks[1] ^ blocks[2] ^ blocks[3]) : (blocks[2] ^ (blocks[1]| (~blocks[3])));
            pos[1] = (i < 16) ? i : (i < 32) ? ((5 * i + 1) % 16) : (i < 48) ? ((3 * i + 5) % 16) : ((7 * i) % 16);
            tblock = blocks[3];
            blocks[3]= blocks[2];
            blocks[2]= blocks[1];
            blocks[1]= blocks[1] + md5_lrot((blocks[0] + pos[0] + k[i] + w[pos[1]]), r[i]);
            blocks[0]= tblock;
        }
        hblock[0] += blocks[0];
        hblock[1] += blocks[1];
        hblock[2] += blocks[2];
        hblock[3] += blocks[3];
    }
    free(msg);
    w32_to_byte(hblock[0], digest);
    w32_to_byte(hblock[1], digest + 4);
    w32_to_byte(hblock[2], digest + 8);
    w32_to_byte(hblock[3], digest + 12);
    for (int8_t z = 0; z < 16; ++z)
        printf("%2.2x", digest[z]);
    printf("\n");
	printf("%s\n", digest);
}