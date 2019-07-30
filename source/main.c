#include <nds.h>

#define DRAW128(vid, x, y, color) ({\
    vid[(y*2) * 256 + (x*2)] = color;\
    vid[(y*2) * 256 + (x*2+1)] = color;\
    vid[(y*2+1) * 256 + (x*2)] = color;\
    vid[(y*2+1) * 256 + (x*2+1)] = color;\
})

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

    while (1) {
        for (x = 0; x < 128; x++) {
            for (y = 0; y < 128; y++) {
                int time = rand();
                DRAW128(videoMemoryMain, x, y, ARGB16(1, 31, (x*2*time) % 32, (y*2) % 32));
                DRAW128(videoMemorySub, x, y, ARGB16(1, (x*2) % 32, 31, (y*2*time) % 32));
            }
        }

        swiWaitForVBlank();
    }
}
