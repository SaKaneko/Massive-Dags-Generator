#pragma once
#include <vector>
#include <string>
#include <filesystem>

namespace MDG {

// Graph Generation

int nodes                              = 40;
double alpha                           = 1.0;
double p                               = 0.50;
const std::vector<double> alpha_loader = {0.25, 0.5, 1.0, 2.0};
const std::vector<int> nodes_loader    = {20, 40, 60, 80, 100};
const std::vector<double> p_loader     = {0, 0.5};
const int iteration                    = 25;
enum output_type { Table, Adjacency };

// File Output

const std::filesystem::path Output_directory = ".";
const std::string prefix                     = "normalgraph";

} // namespace MDG