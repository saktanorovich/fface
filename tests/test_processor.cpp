#include <boost/program_options.hpp>
#include <fface/arguments.hpp>
#include <fface/processor.hpp>
#include <fface/library_loader.hpp>
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>

namespace po = boost::program_options;
namespace fs = std::filesystem;

struct mock_library_loader : public fface::library_loader {
public:
    bool invoked = false;
    std::string libpath;
public:
    void load(const std::string& path) override {
        invoked = true;
        libpath = path;
    }
    detect_faces_fn detector() const override {
        return [](const char*, const char*, const char*) {
            return;
        };
    }
};

po::options_description get_description() {
    po::options_description description("Options");
    description.add_options()
        ("help,h", "Show help message")
        ("dir,d", po::value<std::string>()->required(), "Directory to search for image files")
        ("lib,l", po::value<std::string>()->required(), "Shared library to process images")
        ("mod,m",
            po::value<std::string>()
                ->default_value("./model/fd_yunet_2023mar.onnx")
                ->required(),
            "Path to the DNN model");
    return description;
}

TEST(ProcessorTest, CallsLoadLib) {
    const char* argv[] = {"prog", "--dir", "folder", "--lib", "lib.so", "--mod", "mod.so"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    fface::arguments args(get_description(), argc, const_cast<char**>(argv));

    mock_library_loader library_loader;
    fface::processor proc(args, library_loader);
    proc.process();

    EXPECT_TRUE(library_loader.invoked);
    EXPECT_EQ(library_loader.libpath, "lib.so");
}

TEST(ProcessorTest, ScanDirectoryFindsImages) {
    std::filesystem::path tempDir = std::filesystem::temp_directory_path() / "proc_test";
    std::filesystem::create_directory(tempDir);
    std::ofstream(tempDir / "image.jpg").put('x');

    const char* argv[] = {"prog", "--dir", tempDir.c_str(), "--lib", "lib.so", "--mod", "mod.so"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    mock_library_loader library_loader;

    fface::arguments args(get_description(), argc, const_cast<char**>(argv));
    fface::processor proc(args, library_loader);

    EXPECT_NO_THROW(proc.process());

    std::filesystem::remove_all(tempDir);
}
