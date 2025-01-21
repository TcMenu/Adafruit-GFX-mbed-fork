

file(GLOB SOURCES 
        "${CMAKE_CURRENT_LIST_DIR}/../src/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../src/oled/*.cpp"
)

add_library(AdafruitGFXNativePort ${SOURCES})

target_compile_definitions(AdafruitGFXNativePort
        PUBLIC BUILD_FOR_PICO_CMAKE=1 BUILD_PICO_FORCE_UART=1 IO_LOGGING_DEBUG=1
)

target_include_directories(AdafruitGFXNativePort PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/../src
        ${CMAKE_CURRENT_LIST_DIR}/../Fonts
)

target_link_libraries(AdafruitGFXNativePort PUBLIC pico_stdlib pico_sync IoAbstraction TaskManagerIO tcUnicodeHelper)
