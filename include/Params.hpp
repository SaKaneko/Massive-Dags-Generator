#pragma once
#include <vector>
#include <string>
#include <filesystem>

namespace MDG {

// Graph Generation

const std::vector<double> alpha = {0.5, 1.0, 2.0};
const std::vector<int> nodes    = {20, 40, 60, 80, 100};
const std::vector<double> p     = {0.00, 0.25, 0.50, 0.75, 1.00};
const int iteration             = 3;
enum output_type { Table, Adjacency };

// File Output

const std::filesystem::path Output_directory = ".";
const std::filesystem::path GraphOutputPath  = "./graphv";
const std::string prefix                     = "MdgOut";

} // namespace MDG