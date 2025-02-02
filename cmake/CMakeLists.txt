cmake_minimum_required(VERSION 3.13)

add_library(AdafruitGFXNativePort
        ../src/Adafruit_SSD1306.cpp
        ../src/Adafruit_GFX.cpp
        ../src/oled/AdafruitSSD1306I2c.cpp
        ../src/oled/AdafruitSSD1306Spi.cpp
)

target_compile_definitions(AdafruitGFXNativePort
        PUBLIC BUILD_FOR_PICO_CMAKE=1 BUILD_PICO_FORCE_UART=1 IO_LOGGING_DEBUG=1
)

target_include_directories(AdafruitGFXNativePort PUBLIC
        ../src
)

target_link_libraries(AdafruitGFXNativePort PUBLIC pico_stdlib pico_sync IoAbstraction TaskManagerIO tcUnicodeHelper)
