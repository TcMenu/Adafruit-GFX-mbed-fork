
add_library(AdafruitGFXNativePort
        ../src/Adafruit_SSD1306.cpp
        ../src/Adafruit_GFX.cpp
        ../src/oled/AdafruitSSD1306I2c.cpp
        ../src/oled/AdafruitSSD1306Spi.cpp
)

if (BUILD_FOR_STM32CUBE)
    target_compile_definitions(AdafruitGFXNativePort
            PUBLIC BUILD_FOR_STM32CUBE_CMAKE=1 IO_LOGGING_DEBUG=1
    )
elseif (BUILD_NATIVE_TESTS)
    target_compile_definitions(AdafruitGFXNativePort
            PUBLIC BUILD_FOR_NATIVE_PLATFORM=1 IO_LOGGING_DEBUG=1
    )
else ()
    target_compile_definitions(AdafruitGFXNativePort
            PUBLIC BUILD_FOR_PICO_CMAKE=1 BUILD_PICO_FORCE_UART=1 IO_LOGGING_DEBUG=1
    )
endif ()

target_include_directories(AdafruitGFXNativePort PUBLIC
        ../src/
)

target_link_libraries(AdafruitGFXNativePort PUBLIC IoAbstraction TaskManagerIO tcUnicodeHelper)

# only bring in pico libs when compiling for PicoSDK
if(TARGET pico_stdlib AND TARGET pico_sync)
    message("Build is pico, adding libs")
    target_link_libraries(IoAbstraction PUBLIC pico_stdlib pico_sync)
endif ()
