@echo off
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TEST=ON
cmake --build build --config Release
ctest -C Release --test-dir build --output-on-failure
