```bash
cmake -GNinja -DTARGET_PLATFORM=TEENSY41 -DPython3_EXECUTABLE=$(which python) -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain_teensy41.cmake ..

```