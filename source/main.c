#include <nds.h>

#define DRAW128(vid, x, y, color) ({\
    vid[(y<<8) + x + 8256] = (color);\
})

int main(void) {
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

    int x, y;
    int gradx = 0;
    int grady = 0;
    //int time = 0;
    int step = 0;

    for (;;) {
        // Update state
        //time++;
        //if (time % 3 == 0) step++;
        step++;

        scanKeys();
        uint32 keys = keysHeld();
        if (keys & KEY_UP) gradx++;
        if (keys & KEY_DOWN) gradx--;
        if (keys & KEY_LEFT) grady++;
        if (keys & KEY_RIGHT) grady--;

        // Wait for vblank
        swiWaitForVBlank();

        // Draw
        for (x = 0; x < 128; x++) {
            for (y = 0; y < 128; y++) {
                DRAW128(videoMemoryMain, x, y, ARGB16(1, step+x, gradx+x, gradx));
                DRAW128(videoMemorySub, x, y, ARGB16(1, step+y, grady+x, grady));
            }
        }
    }
}
