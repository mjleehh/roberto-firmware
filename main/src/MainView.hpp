#pragma once

#include <lwip/ip4_addr.h>
#include <string>

namespace roberto {

// ---------------------------------------------------------------------------------------------------------------------

struct MainView {
    MainView();

    esp_err_t setMessage(const std::string &message);

    esp_err_t setWifiStatus(const std::string &status);

    esp_err_t setIp(const ip4_addr &ip);

private:
    esp_err_t draw();

    ip4_addr ip_;
    std::string message_;
    std::string wifiStatus_;
};

// ---------------------------------------------------------------------------------------------------------------------

}
