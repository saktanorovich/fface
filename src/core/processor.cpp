#include <fface/arguments.hpp>
#include <fface/processor.hpp>
#include <fface/library_loader.hpp>
#include <fface/detector/detector.hpp>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <filesystem>
#include <string>
#include <unordered_set>

namespace fs = std::filesystem;

namespace fface {

processor::processor(const fface::arguments& args, fface::library_loader& loader)
    : args(args), loader(loader)
{
}

void processor::process() {
    load_lib();
    scan_dir();
}

void processor::load_lib() {
    loader.load(args.get_lib_path());
}

void processor::scan_dir() {
    fs::path target_dir = args.get_dir_path();
    if (!fs::exists(target_dir) || !fs::is_directory(target_dir)) {
        std::cerr << "Invalid directory: " << target_dir.string() << "\n";
        return;
    }

    std::cout << "Listing Images in: " << target_dir.string() << std::endl;
    std::cout
            << std::left
            << std::setw(FACES_HEADER_WIDTH) << "Faces"
            << std::setw(IMAGE_HEADER_WIDTH) << "Image" << std::endl;

    detect_faces_fn detect_faces = loader.detector();
    std::string mod_path = args.get_mod_path();
    const char* target_path = target_dir.c_str();
    const char* model_path = mod_path.c_str();
    std::vector<std::string> exts = args.get_ext();
    std::unordered_set<std::string> img_ext_set(exts.begin(), exts.end());
    for (const auto& entry : fs::recursive_directory_iterator(target_dir, fs::directory_options::skip_permission_denied)) {
        if (entry.is_regular_file()) {
            std::string image_ext = entry.path().extension().string();
            std::transform(image_ext.begin(), image_ext.end(), image_ext.begin(),
                [](unsigned char c) { return std::tolower(c); }
            );
            if (img_ext_set.count(image_ext)) {
                detect_faces(target_path, model_path, entry.path().c_str());
            }
        }
    }
}

} // namespace fface
