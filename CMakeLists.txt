# pico-pacPlus
# Odyssey 2 / VideoPac+ emulator for RP2040/RP2350

cmake_minimum_required(VERSION 3.13)

set(projectname picoPacPlus)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# ---------------------------------------------------------------------------
# Custom build type: RELEASENODEBUG
# ---------------------------------------------------------------------------
message("* Adding build types...")

set(GCC_RELEASENODEBUG_FLAGS "-O2 -DNDEBUG")

set(CMAKE_ASM_FLAGS_RELEASENODEBUG "${GCC_RELEASENODEBUG_FLAGS}"
    CACHE STRING "Flags used by the ASM compiler during RELEASENODEBUG builds." FORCE)
set(CMAKE_CXX_FLAGS_RELEASENODEBUG "${GCC_RELEASENODEBUG_FLAGS}"
    CACHE STRING "Flags used by the C++ compiler during RELEASENODEBUG builds." FORCE)
set(CMAKE_C_FLAGS_RELEASENODEBUG   "${GCC_RELEASENODEBUG_FLAGS}"
    CACHE STRING "Flags used by the C compiler during RELEASENODEBUG builds." FORCE)
set(CMAKE_EXE_LINKER_FLAGS_RELEASENODEBUG ""
    CACHE STRING "Flags used for linking binaries during RELEASENODEBUG builds." FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_RELEASENODEBUG ""
    CACHE STRING "Flags used by shared libraries linker during RELEASENODEBUG builds." FORCE)

mark_as_advanced(
    CMAKE_CXX_FLAGS_RELEASENODEBUG
    CMAKE_ASM_FLAGS_RELEASENODEBUG
    CMAKE_C_FLAGS_RELEASENODEBUG
    CMAKE_EXE_LINKER_FLAGS_RELEASENODEBUG
    CMAKE_SHARED_LINKER_FLAGS_RELEASENODEBUG)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug
        CACHE STRING "Choose the type of build : None Debug Release RelWithDebInfo MinSizeRel RELEASENODEBUG."
        FORCE)
endif()
message("* Current build type is : ${CMAKE_BUILD_TYPE}")

# ---------------------------------------------------------------------------
# Hardware / board selection
# ---------------------------------------------------------------------------
if(NOT HW_CONFIG)
    set(HW_CONFIG 8 CACHE STRING "Select the hardware configuration for your board")
endif()

if(NOT USE_HSTX)
    set(USE_HSTX 1 CACHE STRING "Use HSTX for display output when possible")
endif()

include("pico_shared/BoardConfigs.cmake")

if(NOT PICO_BOARD)
    set(PICO_BOARD pico2 CACHE STRING "Board type")
    message("PICO_BOARD not set, using default: ${PICO_BOARD}")
endif()

add_definitions(-DNDEBUG)

# ---------------------------------------------------------------------------
# TinyUSB family support + Pico SDK init
# ---------------------------------------------------------------------------
set(FAMILY rp2040)
include($ENV{PICO_SDK_PATH}/lib/tinyusb/hw/bsp/family_support.cmake)

if(ENABLE_PIO_USB EQUAL 0 AND TARGET tinyusb_common_base)
    get_target_property(_tu_defs tinyusb_common_base INTERFACE_COMPILE_DEFINITIONS)
    if(_tu_defs)
        list(REMOVE_ITEM _tu_defs "CFG_TUH_RPI_PIO_USB=1")
        set_target_properties(tinyusb_common_base PROPERTIES
            INTERFACE_COMPILE_DEFINITIONS "${_tu_defs}")
    endif()
endif()

project(${projectname} C CXX ASM)

pico_sdk_init()

if(ENABLE_PIO_USB EQUAL 1)
    message("Checking for PIO USB support...")
    check_and_add_pico_pio_usb_support()
endif()
message("PIO USB support: ${ENABLE_PIO_USB}, using PIO: ${PIO_USB_USE_PIO}")

# ---------------------------------------------------------------------------
# Executable
# ---------------------------------------------------------------------------
add_executable(${projectname}
    main.cpp
    splash.cpp
    DefaultSS444.c
    DefaultSS555.c
)

pico_set_program_name(${projectname} "${projectname}")
pico_set_program_version(${projectname} "0.1")

# ---------------------------------------------------------------------------
# stdio routing
# ---------------------------------------------------------------------------
if(UART_ENABLED EQUAL 0)
    message("Disabling UART output for this board.")
    pico_enable_stdio_uart(${projectname} 0)
else()
    pico_enable_stdio_uart(${projectname} 1)
endif()
pico_enable_stdio_usb(${projectname} 0)

# ---------------------------------------------------------------------------
# Compile options
# ---------------------------------------------------------------------------
target_compile_definitions(${projectname} PRIVATE
    PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1
)

target_compile_options(${projectname} PRIVATE
    -Wno-unused-parameter
    -Wno-unused-function
    -Wno-cast-function-type
    -Wno-maybe-uninitialized
)

# ---------------------------------------------------------------------------
# Project compile definitions (board pins, features, etc.)
# ---------------------------------------------------------------------------
target_compile_definitions(${projectname} PRIVATE
    DVICONFIG=${DVICONFIG}
    SDCARD_PIN_CS=${SD_CS}
    SDCARD_PIN_SCK=${SD_SCK}
    SDCARD_PIN_MOSI=${SD_MOSI}
    SDCARD_PIN_MISO=${SD_MISO}
    SDCARD_PIO=pio1
    SDCARD_SPI=${SD_SPI}
    NES_PIN_CLK=${NES_CLK}
    NES_PIN_DATA=${NES_DATA}
    NES_PIN_LAT=${NES_LAT}
    NES_PIO=${NES_PIO}
    NES_PIN_CLK_1=${NES_CLK_1}
    NES_PIN_DATA_1=${NES_DATA_1}
    NES_PIN_LAT_1=${NES_LAT_1}
    NES_PIO_1=${NES_PIO_1}
    WII_PIN_SDA=${WII_SDA}
    WII_PIN_SCL=${WII_SCL}
    LED_GPIO_PIN=${LED_GPIO_PIN}
    HW_CONFIG=${HW_CONFIG}
    WIIPAD_I2C=${WIIPAD_I2C}
    USE_I2S_AUDIO=${USE_I2S_AUDIO}
    PICO_AUDIO_I2S_DATA_PIN=${PICO_AUDIO_I2S_DATA_PIN}
    PICO_AUDIO_I2S_CLOCK_PIN_BASE=${PICO_AUDIO_I2S_CLOCK_PIN_BASE}
    PICO_AUDIO_I2S_PIO=${PICO_AUDIO_I2S_PIO}
    PICO_AUDIO_I2S_DMA_IRQ=1
    PICO_AUDIO_I2S_CLOCK_PINS_SWAPPED=${PICO_AUDIO_I2S_CLOCK_PINS_SWAPPED}
    PICO_AUDIO_I2S_RESET_PIN=${PICO_AUDIO_I2S_RESET_PIN}
    PICO_AUDIO_I2S_INTERRUPT_PIN=${PICO_AUDIO_I2S_INTERRUPT_PIN}
    PICO_AUDIO_I2S_INTERRUPT_IS_BUTTON=${PICO_AUDIO_I2S_INTERRUPT_IS_BUTTON}
    CFG_TUH_RPI_PIO_USB=${ENABLE_PIO_USB}
    PIO_USB_USE_PIO=${PIO_USB_USE_PIO}
    PSRAM_CS_PIN=${PSRAM_CS_PIN}
    GPIOHSTXD0=${GPIOHSTXD0}
    GPIOHSTXD1=${GPIOHSTXD1}
    GPIOHSTXD2=${GPIOHSTXD2}
    GPIOHSTXCK=${GPIOHSTXCK}
    GPIOHSTXINVERTED=${GPIOHSTXINVERTED}
    ENABLE_VU_METER=${ENABLE_VU_METER}
    WIIPAD_DELAYED_START=${WIIPAD_DELAYED_START}
    ENABLEDVI=1
    DOUBLEFRAMEBUFFER=0
    __O2EM_PICO__
    __O2EM_RELEASE__
)

if(NOT PICO_PLATFORM STREQUAL "rp2040")
    message(STATUS "Using PICO_PIO_USE_GPIO_BASE=1 for non-rp2040 platforms.")
    target_compile_definitions(${projectname} PRIVATE
        PICO_PIO_USE_GPIO_BASE=1
        PICO_STACK_SIZE=3072
    )
endif()

if(DEFINED PICO_DEFAULT_UART_TX_PIN)
    message(STATUS "Using PICO_DEFAULT_UART_TX_PIN: ${PICO_DEFAULT_UART_TX_PIN}")
    target_compile_definitions(${projectname} PRIVATE
        PICO_DEFAULT_UART_TX_PIN=${PICO_DEFAULT_UART_TX_PIN}
        PICO_DEFAULT_UART=0
    )
endif()

if(DEFINED PICO_DEFAULT_UART_RX_PIN)
    message(STATUS "Using PICO_DEFAULT_UART_RX_PIN: ${PICO_DEFAULT_UART_RX_PIN}")
    target_compile_definitions(${projectname} PRIVATE
        PICO_DEFAULT_UART_RX_PIN=${PICO_DEFAULT_UART_RX_PIN}
    )
endif()

if(DEFINED PICO_DEFAULT_UART)
    message(STATUS "Using PICO_DEFAULT_UART: ${PICO_DEFAULT_UART}")
    target_compile_definitions(${projectname} PRIVATE
        PICO_DEFAULT_UART=${PICO_DEFAULT_UART}
    )
endif()

# ---------------------------------------------------------------------------
# PIO USB (optional)
# ---------------------------------------------------------------------------
if(ENABLE_PIO_USB EQUAL 1)
    if(NOT PIO_DP_PLUS_PIN EQUAL -1)
        message(STATUS "Using PIO USB DP pin: ${PIO_DP_PLUS_PIN}")
        target_compile_definitions(${projectname} PRIVATE
            PICO_DEFAULT_PIO_USB_DP_PIN=${PIO_DP_PLUS_PIN}
        )
    endif()
    message("Adding target link libraries for PIO USB support.")
    target_link_libraries(${projectname} PRIVATE tinyusb_pico_pio_usb)
endif()

# ---------------------------------------------------------------------------
# Link
# ---------------------------------------------------------------------------
target_link_libraries(${projectname} PRIVATE
    pico_stdlib
    pico_multicore
    hardware_dma
    hardware_pio
    hardware_i2c
    hardware_interp
    hardware_timer
    hardware_clocks
    hardware_pwm
    hardware_flash
    dvi
    util
    o2em2
    tinyusb_host
    tinyusb_board
    pico_shared
    xinput_host
)

if(PICO_BOARD STREQUAL "pico_w" OR PICO_BOARD STREQUAL "pico2_w")
    target_link_libraries(${projectname} PRIVATE pico_cyw43_arch_none)
endif()

target_include_directories(${projectname} PRIVATE
    pico_lib
    pico_shared
    o2em2-1.51
    tusb_xinput
    .
)

add_subdirectory(o2em2-1.51)
add_subdirectory(pico_lib/dvi)
add_subdirectory(pico_lib/util)
add_subdirectory(pico_shared)
add_subdirectory(tusb_xinput xinput_host)

target_link_options(${projectname} PRIVATE -Xlinker --print-memory-usage --data-sections)

# Optional: relink this emulator to run from the bootloader's application
# partition (for use with pico_emuLoader). Off by default so standalone builds
# are unchanged. Enable with: cmake -DBUILD_FOR_BOOTLOADER=ON ..
if(BUILD_FOR_BOOTLOADER)
    include("pico_shared/BootPartition.cmake")
    if(DEFINED BUILD_FOR_BOOTLOADER_SLOT)
        frens_link_to_pinned_slot(${projectname} ${BUILD_FOR_BOOTLOADER_SLOT})
    else()
        frens_offset_for_bootloader(${projectname})
    endif()
endif()

pico_add_extra_outputs(${projectname})
