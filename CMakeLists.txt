set ( sourcefiles 
"main.c"
"device/system.h" "device/system.c"
"device/graphic.h" "device/graphic.c"
"device/rom.h" "device/rom.c"
"cpu/operations.h" "cpu/core.h"
"cpu/operations.c" "cpu/core.c"
"device/input.h" "device/input.c"
"data/o.h" "device/sound.c" "device/sound.h"
)

set(PROJECT pico_nu_pogodi)
cmake_minimum_required(VERSION 3.12)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(${PROJECT} C CXX ASM)
pico_sdk_init()



add_executable(${PROJECT} ${sourcefiles})
add_subdirectory(lib/pico-display-drivs/ili9341)
add_subdirectory(lib/pico-display-drivs/gfx)
#add_subdirectory(pico-extras)
target_include_directories(${PROJECT} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

target_link_libraries(${PROJECT} pico_stdlib hardware_pio pico_multicore ili9341 gfx)
target_include_directories(${PROJECT} PRIVATE "cpu" "gw" ".")
pico_generate_pio_header(${PROJECT} ${CMAKE_CURRENT_LIST_DIR}/device/sound.pio)

#pico_generate_pio_header(${PROJECT} device/beep_sound.pio)
target_link_options(${PROJECT} PUBLIC LINKER:-Map=${PROJECT}.map)
pico_enable_stdio_usb(${PROJECT} 1)
pico_enable_stdio_uart(${PROJECT} 0)
pico_add_extra_outputs(${PROJECT})

