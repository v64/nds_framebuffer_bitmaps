#include <nds.h>

#define SCR_WIDTH 256
#define SCR_HEIGHT 192

#define DRAW128(vid, x, y, color) ({\
    vid[(y<<8) + x + 8256] = (color);\
})

#define DRAW(vid, x, y, color) ({\
    vid[y * SCR_WIDTH + x] = (color);\
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
    int redraw = 1;

    for (;;) {
        scanKeys();
        uint32 keys = keysHeld();
        if (keys & KEY_UP)    { gradx++; redraw = 1; }
        if (keys & KEY_DOWN)  { gradx--; redraw = 1; }
        if (keys & KEY_LEFT)  { grady++; redraw = 1; }
        if (keys & KEY_RIGHT) { grady--; redraw = 1; }

        // Wait for vblank
        swiWaitForVBlank();

        // Draw
        if (redraw) {
            for (x = 0; x < SCR_WIDTH; x++) {
                for (y = 0; y < SCR_HEIGHT; y++) {
                    DRAW(videoMemoryMain, x, y, ARGB16(1, gradx*y+x, x, grady));
                    DRAW(videoMemorySub, x, y, ARGB16(1, grady*x+y, gradx, y));
                }
            }

            redraw = 0;
        }
    }
}
