# MegaLines

A cross-platform application built with Qt for managing and playing MegaLines. This project uses CMake for build configuration.

## License

All rights reserved.

This code is proprietary and is provided for viewing purposes only. Redistribution, modification, commercial or non-commercial use of this code, in whole or in part, is strictly prohibited without prior written permission from the author.

## Requirements

### Linux
- CMake 3.16 or higher
- Qt5 or Qt6 with components: `Core`, `Quick`, `Sql`
- A C++17-compatible compiler (e.g., GCC, Clang)

### Windows
- CMake 3.16 or higher
- Qt5 or Qt6 with components: `Core`, `Quick`, `Sql`
- A C++17-compatible compiler (e.g., MSVC)

---

## Build Instructions

### Linux
```bash
# Clone the repository
git clone https://github.com/Segaman8/MegaLines.git
cd MegaLines

# Create a build directory
mkdir build && cd build

# Configure the project
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
make

# Run the application
./MegaLines

```

### Windows (Using PowerShell)
```bash
# Clone the repository
git clone https://github.com/yourusername/MegaLines.git
cd MegaLines

# Create a build directory
mkdir build
cd build

# Configure the project
cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release

# Build the project
nmake

# Run the application
MegaLines.exe
```
