#include <fface/arguments.hpp>
#include <fface/processor.hpp>
#include <fface/library_loader.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
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

    fface::dynamic_library_loader library_loader;
    fface::arguments arguments(description, argc, argv);
    fface::processor processor(arguments, library_loader);
    processor.process();

    return 0;
}
