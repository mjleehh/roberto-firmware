set(COMPONENT_SRCDIRS src)
set(COMPONENT_ADD_INCLUDEDIRS src)

set(COMPONENT_REQUIRES
    nvs_flash
    esp_http_client
    mfl_http_client
    mfl_http
    mfl_wifi
    mfl_access_point
    mfl_esp_httpd
    mfl_color_display
    tcp_transport
    spiffs
    modern_json)

register_component()

spiffs_create_partition_image(webapp ${PROJECT_SOURCE_DIR}/www FLASH_IN_PROJECT)

