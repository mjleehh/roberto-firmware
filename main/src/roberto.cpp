#include <mfl/Wifi.hpp>
#include <esp_log.h>
#include <nvs_flash.h>
#include <mfl/EspHttpd.hpp>
#include <mfl/httpd/Router.hpp>
#include "MainView.hpp"
#include <mfl/ColorDisplay.hpp>
#include <tojson.hpp>
#include <mfl/http/Client.hpp>
#include "pinFromInt.hpp"
#include "ProgramModes.hpp"
#include "waitForDebugger.hpp"
#include "displayTest.hpp"

#include <iostream>
#include <esp_http_client.h>

#include <nlohmann/adl_serializer.hpp>

#include <sdkconfig.h>
#include <lvgl/lvgl.h>
#include <esp_freertos_hooks.h>
#include <mfl/color_display/LvglAdapter.hpp>
#include <mfl/AccessPoint.hpp>

const std::string HOME_ADDRESS = "http://" CONFIG_ROBERTO_HOME_ADDRESS ":3001/api/" CONFIG_ROBERTO_ID;
const std::string ROBERTO_AP_NAME = "roberto-" CONFIG_ROBERTO_NAME;

const char* tag = "roberto";

using namespace mfl;
using namespace roberto;

const ProgramMode mode = ProgramMode::factoryDefault;

extern "C" void app_main() {
    waitForDebugger();

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

    switch (mode) {
        case ProgramMode::normal: {
            httpd::Router router;
            EspHttpd app(router, 80);
            color_display::LvglAdapter lvgl(display);
            Wifi wifi(CONFIG_ROBERTO_NAME, CONFIG_ROBERTO_WIFI_SSID, CONFIG_ROBERTO_WIFI_PASSWORD);
            wifi.start(
                    [&wifi, &app, &router](const ip4_addr &addr) {
                        ESP_LOGI(tag, "wifi started successfully");
                        wifi.addService("roberto", mfl::Wifi::Protocol::tcp, 80, "foo");
                        //                mainView.setIp(addr);
                        //                mainView.setWifiStatus("c");

                        std::string body = "{ \"msg\" : \"foobar\" }";

                        //                http::Client homeClient(http::Method::put, HOME_ADDRESS);
                        //                homeClient
                        //                    .body(body)
                        //                    .request<std::string>();

                        app.start();

                        router.put("/message", [](mfl::httpd::Context<nlohmann::json> &ctx) {
                            try {
                                std::cout << ctx.body.dump() << std::endl;
                                auto s = ctx.body["msg"].get<std::string>();
                                std::cout << s << std::endl;
                                //                        mainView.setMessage(s);
                            } catch (...) {
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

            for (;;) {
                vTaskDelay(10 / portTICK_PERIOD_MS);
                lvgl.runTasks(10);
            }
        }
        case ProgramMode::factoryDefault: {
            httpd::Router router;
            EspHttpd app(router, 80);
            AccessPoint accessPoint(CONFIG_ROBERTO_NAME, ROBERTO_AP_NAME, CONFIG_ROBERTO_AP_PASSWORD);
            accessPoint.start([&accessPoint, &app, &router]{
                accessPoint.addService("roberto-direct", mfl::AccessPoint::Protocol::tcp, 80, "foo");
                app.start();
                router.get("/api/devinfo", [](mfl::httpd::Context<nlohmann::json> &ctx){
                    ctx.res.body = nlohmann::json();
                });
                router.get("/message", [](mfl::httpd::Context<std::string> &ctx){
                    ctx.res.body = "hello bob";
                });
                router.put("/message", [](mfl::httpd::Context<std::string> &ctx){
                    ctx.res.body = "hello charles";
                });
            });
            for (;;) {
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
        }
        case ProgramMode::displayTest: {
            displayTest(display);
            for (;;) {
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
        }
        default: {
            for (;;) {
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
        }
    }
}
