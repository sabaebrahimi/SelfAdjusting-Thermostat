#include <string.h>
#include <stdint.h>

#define ARIAL_22PT												
#define ARIALFONT_HIGHT_22PT							30
#define CHAR_SPACE_22PT										2

// Font data for Arial 22pt
extern const uint8_t arial_22pt[];
extern const uint16_t ArialFontInfo_22pt[][2];


void GetFontInfo_22pt(uint16_t* pBuffer,unsigned char ASCII);

