#include <string.h>
#include <stdint.h>

#define ARIAL_30PT												
#define ARIALFONT_HIGHT_30PT							40
#define CHAR_SPACE_30PT										3

// Font data for Arial 30pt
extern const uint8_t arial_30pt[];
extern const uint16_t ArialFontInfo_30pt[][2];


void GetFontInfo_30pt(uint16_t* pBuffer,unsigned char ASCII);

