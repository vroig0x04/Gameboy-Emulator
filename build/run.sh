
#!/bin/bash 
# shebang to use bash

BUILD_TYPE="Realese" # set by default

if [ "$1" == "d" ]; then
    BUILD_TYPE="Debug"
fi

cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..

cmake --build .

# ensure last command was successful
if [ $? -eq 0 ]; then 
    
    if [ "$BUILD_TYPE" == "Debug" ]; then 
        echo "Executing gameboy emulator in debug mode"
        gdb ./gameboy
    else
        echo "Executing gameboy emulator normal mode"
        ./gameboy
    fi
else 
    echo "Failure during compilation time"
fi