add_library(teensy_core STATIC

    # Startup / system
    teensy4/startup.c
    teensy4/clockspeed.c
    teensy4/delay.c
    teensy4/yield.cpp
    #teensy4/imxrt.cpp
    teensy4/EventResponder.cpp

    # Arduino core
    teensy4/Print.cpp
    teensy4/Stream.cpp
    teensy4/WMath.cpp
    teensy4/WString.cpp
    #teensy4/abi.cpp
    teensy4/new.cpp

    # Utilities / system
    teensy4/eeprom.c
    teensy4/rtc.c
    #teensy4/smalloc.c
    teensy4/nonstd.c

    # Pins / GPIO
    #teensy4/pins_teensy.c

    # Serial / UART
    teensy4/HardwareSerial.cpp
    teensy4/HardwareSerial1.cpp
    teensy4/HardwareSerial2.cpp
    teensy4/HardwareSerial3.cpp
    teensy4/HardwareSerial4.cpp
    teensy4/HardwareSerial5.cpp
    teensy4/HardwareSerial6.cpp
    teensy4/HardwareSerial7.cpp

    # USB SERIAL
    teensy4/usb.c
    teensy4/usb_desc.c
    #teensy4/usb_mem.c
    teensy4/usb_serial.c
)

target_include_directories(teensy_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/teensy4
)

target_compile_definitions(teensy_core PUBLIC

    __IMXRT1062__
    TEENSY41
    ARDUINO_TEENSY41
    ARDUINO=10805
    TEENSYDUINO=158

    F_CPU=600000000

    USB_SERIAL
)

target_compile_options(teensy_core PUBLIC

    -fno-exceptions
    -fno-rtti
    -fno-threadsafe-statics
    -fpermissive
)
