# Build Configurations
---
Try to not diverge from the examples below.
```bash
cmake -S . -B build/Desktop_Debug -DCMAKE_BUILD_TYPE=Debug # for development build
cmake -S . -B build/Desktop_Release -DCMAKE_BUILD_TYPE=Release # for release build
```
For building the binary:
```bash
cmake --build ./build/Desktop_[Debug|Release]
```