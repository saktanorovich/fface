#pragma once
#include <boost/program_options.hpp>
#include <string>
#include <vector>

namespace po = boost::program_options;

namespace fface {

class arguments {
private:
    po::variables_map variables_map;
    po::options_description description;
    std::string dir_path;
    std::string lib_path;
    std::string mod_path;
private:
    void parse(int argc, char* argv[]);
public:
    arguments(const po::options_description& description, int argc, char* argv[]);
public:
    std::string get_dir_path() const;
    std::string get_lib_path() const;
    std::string get_mod_path() const;
    std::vector<std::string> get_ext() const;

    bool help() const;
};

} // namespace fface
