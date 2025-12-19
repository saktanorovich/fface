#pragma once
#include <fface/detector/detector.hpp>
#include <iostream>
#include <stdexcept>
#include <dlfcn.h>

namespace fface {

struct library_loader {
    virtual void load(const std::string& lib_path) = 0;
    virtual detect_faces_fn detector() const = 0;
    virtual ~library_loader() = default;
};

struct dynamic_library_loader : public library_loader {
private:
    void* lib_handle;
    detect_faces_fn detector_fn;
public:
    void load(const std::string& lib_path) override {
        std::cout << "Loading library: " << lib_path << std::endl;
        auto handle = dlopen(lib_path.c_str(), RTLD_LAZY);
        if (!handle) {
            std::cerr << "Failed to load library: " << std::string(dlerror()) << std::endl;
            exit(1);
        }
        lib_handle = handle;
        dlerror();
        auto fn_handle = dlsym(handle, "detect_faces");
        if (!fn_handle) {
            std::cerr << "detect_faces function not found: " << std::string(dlerror()) << std::endl;
            exit(1);
        }
        detector_fn = reinterpret_cast<detect_faces_fn>(fn_handle);
    }
    detect_faces_fn detector() const override {
        return detector_fn;
    }
    ~dynamic_library_loader() override {
        if (lib_handle) {
            dlclose(lib_handle);
        }
    }
};

} // namespace fface
