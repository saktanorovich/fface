#include <fface/arguments.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace po = boost::program_options;

namespace fface {

arguments::arguments(const po::options_description& description, int argc, char* argv[])
    : description(description)
{
    parse(argc, argv);
}

void arguments::parse(int argc, char* argv[]) {
    auto usage = [&](std::ostream& os, const std::string& opt) -> std::ostream& {
        os << "Usage: " << opt << " [OPTIONS]" << std::endl << description << std::endl;
        return os;
    };
    try {
        auto parsed_options = po::parse_command_line(argc, argv, description);
        po::store(parsed_options, variables_map);

        if (variables_map.count("help")) {
            usage(std::cout, argv[0]);
            exit(0);
        }

        po::notify(variables_map);

        dir_path = variables_map["dir"].as<std::string>();
        lib_path = variables_map["lib"].as<std::string>();
        mod_path = variables_map["mod"].as<std::string>();
    } catch (const po::error& e) {
        std::cerr << "Error: " << e.what() << "\n\n";
        usage(std::cerr, argv[0]);
        exit(1);
    }
}

std::string arguments::get_dir_path() const {
    return dir_path;
}

std::string arguments::get_lib_path() const {
    return lib_path;
}

std::string arguments::get_mod_path() const {
    return mod_path;
}

std::vector<std::string> arguments::get_ext() const {
    static const std::vector<std::string> ext = std::vector<std::string> {
        ".jpg", ".jpeg", ".png", ".bmp", ".tiff", ".tif"
    };
    return ext;
}

bool arguments::help() const {
    return variables_map.count("help") > 0;
}

} //namespace fface
