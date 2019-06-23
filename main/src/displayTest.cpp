#include <esp_log.h>
#include "displayTest.hpp"

namespace roberto {

// ---------------------------------------------------------------------------------------------------------------------

void displayTest(mfl::ColorDisplay &display) {
    const int boardSize = 14;
    const int border = 4;
    auto *chess = new lv_color_t[boardSize * boardSize];
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

    display.fill(0, 0, 239, 319, LV_COLOR_MAKE(255, 0, 0));

    display.fill(50, 50, 100, 100, LV_COLOR_MAKE(0, 0, 255));

    ESP_LOGI(TAG, "drawing bits n pieces");
    display.fill(10, 0, 11, 319, LV_COLOR_MAKE(255, 255, 0));
    display.fill(30, 0, 30, 319, LV_COLOR_MAKE(255, 255, 0));

    ESP_LOGI(TAG, "drawing chess");
    display.flush(3, 50, 15, 62, chess);

    ESP_LOGI(TAG, "drawing dots");
    const int XRES = 239;
    const int YRES = 319;
    for (int x = 0; x < XRES; x += 10) {
        for (int y = 0; y < YRES; y += 10) {
            display.fill(x, y, x + 1, y + 1, LV_COLOR_MAKE(0, 255, 0));
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

}
