add_library(o2em2 INTERFACE)

target_sources(o2em2 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/cpu.c
    ${CMAKE_CURRENT_LIST_DIR}/vmachine.c
    ${CMAKE_CURRENT_LIST_DIR}/vdc.c
    ${CMAKE_CURRENT_LIST_DIR}/vpp.c
    ${CMAKE_CURRENT_LIST_DIR}/audio.c
    ${CMAKE_CURRENT_LIST_DIR}/keyboard.c
    ${CMAKE_CURRENT_LIST_DIR}/voice.c
    ${CMAKE_CURRENT_LIST_DIR}/crc32.c
    ${CMAKE_CURRENT_LIST_DIR}/cset.c
    ${CMAKE_CURRENT_LIST_DIR}/vpp_cset.c
    ${CMAKE_CURRENT_LIST_DIR}/table.c
    ${CMAKE_CURRENT_LIST_DIR}/timefunc.c
    ${CMAKE_CURRENT_LIST_DIR}/bios.c
    ${CMAKE_CURRENT_LIST_DIR}/roms.c
    ${CMAKE_CURRENT_LIST_DIR}/score.c
    ${CMAKE_CURRENT_LIST_DIR}/o2em_pico.c
)

target_compile_definitions(o2em2 INTERFACE
    __O2EM_PICO__
    __O2EM_RELEASE__
)

target_include_directories(o2em2 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)
