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
    for(x = 0; x < 256; x++) {
        for(y = 0; y < 256; y++) {
            videoMemoryMain[x + y * 256] = ARGB16(1, 31, 0, 0);
            videoMemorySub[x + y * 256] = ARGB16(1, 0, 0, 31);
        }
    }

    while(1) {
        swiWaitForVBlank();
    }
}
