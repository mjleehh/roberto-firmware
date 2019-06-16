#pragma once

#include <sdkconfig.h>

namespace roberto {

// ---------------------------------------------------------------------------------------------------------------------

void waitForDebugger() {
    #ifdef CONFIG_ROBERTO_WAIT_FOR_DEBUGGER
        volatile bool waiting = true;
        while (waiting) {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    #endif
}

// ---------------------------------------------------------------------------------------------------------------------

}
