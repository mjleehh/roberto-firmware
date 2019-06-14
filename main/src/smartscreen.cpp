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

void IRAM_ATTR lv_tick_task() {
    lv_tick_inc(portTICK_RATE_MS);
}

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
    esp_register_freertos_tick_hook(lv_tick_task);

    Wifi wifi(CONFIG_ROBERTO_NAME, CONFIG_ROBERTO_WIFI_SSID, CONFIG_ROBERTO_WIFI_PASSWORD);
    httpd::Router router;
    EspHttpd app(router, 80);
//    smartscreen::MainView mainView(display);

//    mainView.setMessage("hello bob");
//    mainView.setWifiStatus("-");

    //

    lv_obj_t * page = lv_page_create(NULL, NULL);
    lv_scr_load(page);

    /*Create Label on the currently active screen*/
    lv_obj_t * label1 =  lv_label_create(lv_scr_act(), NULL);

    /*Modify the Label's text*/
    lv_label_set_text(label1, "Hello world!");

    /* Align the Label to the center
     * NULL means align on parent (which is the screen now)
     * 0, 0 at the end means an x, y offset after alignment*/
    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);

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
        vTaskDelay(100);
    }
}
