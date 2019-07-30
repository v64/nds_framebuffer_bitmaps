#include <nds.h>

int main(void) {
    int x, y;

    //set the mode to allow for an extended rotation background
    videoSetMode(MODE_5_2D);
    videoSetModeSub(MODE_5_2D);

    //allocate a vram bank for each display
    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankC(VRAM_C_SUB_BG);

    //create a background on each display
    int bgMain = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
    int bgSub = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);

    u16* videoMemoryMain = bgGetGfxPtr(bgMain);
    u16* videoMemorySub = bgGetGfxPtr(bgSub);

    //initialize it with a color
    for(x = 0; x < 256; x+=2) {
        for(y = 0; y < 256; y+=2) {
            videoMemoryMain[y * 256 + x]         = ARGB16(1, 31, x % 32, y % 32);
            videoMemoryMain[y * 256 + (x+1)]     = ARGB16(1, 31, x % 32, y % 32);
            videoMemoryMain[(y+1) * 256 + x]     = ARGB16(1, 31, x % 32, y % 32);
            videoMemoryMain[(y+1) * 256 + (x+1)] = ARGB16(1, 31, x % 32, y % 32);

            videoMemorySub[y * 256 + x]         = ARGB16(1, x % 32, 31, y % 32);
            videoMemorySub[y * 256 + (x+1)]     = ARGB16(1, x % 32, 31, y % 32);
            videoMemorySub[(y+1) * 256 + x]     = ARGB16(1, x % 32, 31, y % 32);
            videoMemorySub[(y+1) * 256 + (x+1)] = ARGB16(1, x % 32, 31, y % 32);
        }
    }

    while(1) {
        swiWaitForVBlank();
    }
}
