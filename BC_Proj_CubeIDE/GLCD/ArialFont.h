#include <string.h>
#include <stdint.h>

#define ARIAL_18PT
#define ARIALFONT_HIGHT_18PT						24
#define CHAR_SPACE_18PT									2

// Font data for Arial 18pt
extern const uint8_t arial_18pt[];
extern const uint16_t ArialFontInfo[][2];
//extern const FONT_INFO arial_18ptFontInfo;

void GetFontInfo_18pt(uint16_t* pBuffer,unsigned char ASCII);

