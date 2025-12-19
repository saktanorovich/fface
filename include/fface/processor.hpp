#pragma once
#include <fface/arguments.hpp>
#include <fface/library_loader.hpp>

namespace fface {

class processor {
private:
    fface::arguments args;
    fface::library_loader& loader;
private:
    void load_lib();
    void scan_dir();
public:
    processor(const fface::arguments& args, fface::library_loader& loader);
    void process();
};

} // namespace fface
