#include <nds.h>

#define SCR_WIDTH 256
#define SCR_HEIGHT 192
#define FX_CONST 100

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
    int coordx = SCR_WIDTH / 2;
    int coordy = SCR_HEIGHT / 2;
    int redraw = 1;
    int touched = 0;
    int autochg = 0;
    touchPosition touch;

    for (;;) {
        scanKeys();
        uint32 keys = keysHeld();
        touchRead(&touch);

        if (keys & KEY_UP)    { gradx++; redraw = 1; }
        if (keys & KEY_DOWN)  { gradx--; redraw = 1; }
        if (keys & KEY_LEFT)  { grady++; redraw = 1; }
        if (keys & KEY_RIGHT) { grady--; redraw = 1; }

        if (keys & KEY_TOUCH) {
            coordx = touch.px;
            coordy = touch.py;
            redraw = 1;
            touched = 1;
        }

        if (keys & KEY_A) { autochg = 1; }
        if (keys & KEY_B) { autochg = 0; }

        if (autochg) {
            if (autochg % FX_CONST < FX_CONST/2) {
                gradx++;
                grady--;
            } else {
                gradx--;
                grady++;
            }

            autochg++;
            redraw = 1;
        }

        // Wait for vblank
        swiWaitForVBlank();

        // Draw
        if (redraw) {
            for (x = 0; x < SCR_WIDTH; x++) {
                for (y = 0; y < SCR_HEIGHT; y++) {
                    int dist = abs(x - coordx) + abs(y - coordy);
                    if (touched && dist <= (autochg>0?autochg:(FX_CONST/3)) % FX_CONST) {
                        DRAW(videoMemoryMain, x, y, ARGB16(1, x, dist, y));
                        DRAW(videoMemorySub, x, y, ARGB16(1, y, dist, x));
                    } else {
                        DRAW(videoMemoryMain, x, y, ARGB16(1, gradx*y+x, x, grady));
                        DRAW(videoMemorySub, x, y, ARGB16(1, grady*x+y, y, gradx));
                    }
                }
            }

            redraw = 0;
        }
    }
}
