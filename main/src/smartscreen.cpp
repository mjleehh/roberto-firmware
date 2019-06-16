#include <mfl/Wifi.hpp>
#include <esp_log.h>
#include <nvs_flash.h>
#include <mfl/EspHttpd.hpp>
#include <mfl/httpd/Router.hpp>
#include "MainView.hpp"
#include <mfl/ColorDisplay.hpp>
#include <tojson.hpp>
#include <mfl/http/Client.hpp>

#include <iostream>
#include <esp_http_client.h>

#include <nlohmann/adl_serializer.hpp>

#include <sdkconfig.h>
#include <lvgl/lvgl.h>
#include <esp_freertos_hooks.h>

const std::string HOME_ADDRESS = "http://" CONFIG_ROBERTO_HOME_ADDRESS ":3001/api/" CONFIG_ROBERTO_ID;

const mfl::Wifi::Bssid bssid = {0xc0, 0x25, 0x06, 0xa8, 0x90, 0x37};

const char* tag = "main";

template<int pin>
constexpr gpio_num_t pinFromInt() {
    static_assert(pin >=  GPIO_NUM_0, "pin number can not be nagative");
    static_assert(pin < GPIO_NUM_MAX, "pin number is larger than available pins");
    return (gpio_num_t) pin;
}

using namespace mfl;
using namespace nlohmann;

//void IRAM_ATTR lv_tick_task() {
//    lv_tick_inc(portTICK_RATE_MS);
//}

extern "C" void app_main() {
    ESP_ERROR_CHECK(nvs_flash_init());
    tcpip_adapter_init();

    color_display::ColorDisplayConfig displayConfig =  {
            .clockPin      = pinFromInt<CONFIG_ROBERTO_DISPLAY_CLOCK_PIN>(),
            .dataPin       = pinFromInt<CONFIG_ROBERTO_DISPLAY_DATA_PIN>(),
            .chipSelectPin = pinFromInt<CONFIG_ROBERTO_DISPLAY_DEVICE_SELECT_PIN>(),
            .commandPin    = pinFromInt<CONFIG_ROBERTO_DISPLAY_COMMAND_PIN>(),
            .resetPin      = pinFromInt<CONFIG_ROBERTO_DISPLAY_RESET_PIN>(),
    };

    ColorDisplay display(displayConfig);
//    esp_register_freertos_tick_hook(lv_tick_task);

    Wifi wifi(CONFIG_ROBERTO_NAME, CONFIG_ROBERTO_WIFI_SSID, CONFIG_ROBERTO_WIFI_PASSWORD);
    httpd::Router router;
    EspHttpd app(router, 80);

    ESP_LOGI(tag, "CALLING FILL");

    vTaskDelay(500 / portTICK_PERIOD_MS);

    const int boardSize = 14;
    const int border = 4;
    lv_color_t* chess = new lv_color_t[boardSize * boardSize];
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
    display.fill(5 ,5, 20, 20, LV_COLOR_MAKE(128, 0, 0));
    display.fill(20,20,40, 40, lv_color_t{.red = 0x0f});
    display.fill(0, 0, 5, 8, lv_color_t{.blue = 0x04});

    display.flush(3, 50, 15, 62, chess);

    ESP_LOGI(tag, "CALLED FILL");

//    smartscreen::MainView mainView(display);

//    mainView.setMessage("hello bob");
//    mainView.setWifiStatus("-");

    //

    /*
    wifi.start(
            [&wifi, &app, &router](const ip4_addr &addr) {
                ESP_LOGI(tag, "wifi started successfully");
                wifi.addService("screen", mfl::Wifi::Protocol::tcp, 80, "foo");
//                mainView.setIp(addr);
//                mainView.setWifiStatus("c");

                std::string body = "{ \"msg\" : \"foobar\" }";

//                http::Client homeClient(http::Method::put, HOME_ADDRESS);
//                homeClient
//                    .body(body)
//                    .request<std::string>();

                app.start();

                router.put("/message", [](mfl::httpd::Context<nlohmann::json>& ctx){
                    try {
                        std::cout << ctx.body.dump() << std::endl;
                        auto s = ctx.body["msg"].get<std::string>();
                        std::cout << s << std::endl;
//                        mainView.setMessage(s);
                    } catch(...) {
                        std::cout << "bad request" << std::endl;
                    }

                    nlohmann::json res = {};
                    ctx.res.body = res;
                });
            },
            [] {
                ESP_LOGI(tag, "failed to start wifi");
//                mainView.setWifiStatus("f");
            }
    );
     */

    for (;;) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
//        lv_tick_inc(10);
//        lv_task_handler();
    }
}
