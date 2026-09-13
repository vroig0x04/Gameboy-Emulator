# build dir is required
# cmake .. from build
# camke --build . to compile

cmake_minimum_required(VERSION 3.10)

project(GAMEBOY C)

find_package(SDL3 REQUIRED)

include_directories(src/include
        src/lib
)

# first name of list -> executable
add_executable(gameboy 
        src/main.c 
        src/alu_SM83.c 
        src/flags_SM83.c 
        src/mmu.c 
        src/sharp_SM83.c
        src/ppu.c 
        src/apu.c
)

target_link_libraries(gameboy PRIVATE SDL3::SDL3)