# fface Project

**fface** — a lightweight C++ demo app for face detection.

---

## 📋 Prerequisites

- **Operating System**: macOS (arm64)
- **Compiler**: g++ or clang with C++17 support
- **Build System**: CMake ≥ 3.30
- **Tools**:
  - `make` for building
  - `git` (to clone the repository)

---

## 📚 Required Libraries

- **Standard C++ Library (std)** — core language support
- **OpenCV** — image processing and computer vision
- **Boost** — utility libraries (filesystem, program options, etc.)
- **JsonCpp** — JSON parsing and serialization
- **GoogleTest (gtest)** — unit testing framework

---

## 🚀 How to Run

1. **Clone the repository**
   ```bash
   git clone https://github.com/saktanorovich/fface.git
   cd fface
2. **Build the project**
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ctest
3. **Run the application**
   ```bash
   cd ../
   ./bin/fface -d ./data -l ./bin/lib/detector.so

   # get help messages
   ./bin/fface -h

Assume the images are stored in data folder.