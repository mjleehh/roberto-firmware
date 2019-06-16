#include "displayTest.hpp"

namespace roberto {

// ---------------------------------------------------------------------------------------------------------------------

void displayTest(mfl::ColorDisplay &display) {
    const int boardSize = 14;
    const int border = 4;
    lv_color_t *chess = new lv_color_t[boardSize * boardSize];
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (i < border) {
                chess[i * boardSize + j] = LV_COLOR_MAKE(255, 0, 0);
            } else if (i > boardSize - border) {
                chess[i * boardSize + j] = LV_COLOR_MAKE(255, 255, 0);
            } else if (j < border) {
                chess[i * boardSize + j] = LV_COLOR_MAKE(164, 66, 244);
            } else if (j > boardSize - border) {
                chess[i * boardSize + j] = LV_COLOR_MAKE(66, 244, 238);
            } else {
                chess[i * boardSize + j] = (i + j) % 2 == 0
                                           ? LV_COLOR_MAKE(0, 0, 0)
                                           : LV_COLOR_MAKE(255, 255, 255);
            }
        }
    }

    display.fill(0, 0, 127, 159, LV_COLOR_MAKE(0, 0, 52));
    display.fill(5, 5, 20, 20, LV_COLOR_MAKE(128, 0, 0));
    display.fill(20, 20, 40, 40, lv_color_t{.red = 0x0f});
    display.fill(0, 0, 5, 8, lv_color_t{.blue = 0x04});

    display.flush(3, 50, 15, 62, chess);
}

// ---------------------------------------------------------------------------------------------------------------------

}
