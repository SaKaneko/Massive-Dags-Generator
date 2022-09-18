#include <iostream>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <vector>
#include <set>
#include <queue>
#include <random>
#include "Params.hpp"
namespace fs = std::filesystem;

namespace MDG {

std::random_device rd;
std::mt19937 gen(rd());

int random(int low, int high) {
  std::uniform_int_distribution<> dist(low, high);
  return dist(gen);
}

struct Node {
  std::vector<int> parents;
  std::set<int> ancestors;
  int rank;
  bool operator<(const struct Node& other) const {
    return rank < other.rank;
  }
};

struct Graph {
  std::vector<Node> N;

  void GenerateRandomGraphfromParam(int nodes_in, double alpha_in,
                                    double p_in) {
    N.resize(nodes_in);
    srand(time(NULL));
    double rankmx = sqrt(nodes_in) / alpha_in;
    int ranks     = random(1, 2 * rankmx + 1);
    std::vector<std::vector<int>> rankvec(ranks, std::vector<int>(0));

    for (int i = 0; i < nodes_in; i++) {
      int noderank = random(0, ranks - 1);
      N[i].rank    = noderank;
    }
    std::sort(N.begin(), N.end());
    for (int i = 0; i < nodes_in; i++) {
      int noderank = N[i].rank;
      rankvec[noderank].push_back(i);
    }
    for (int i = 0; i < ranks; i++) {
      for (auto& j : rankvec[i]) {
        for (int k = i + 1; k < ranks; k++) {
          for (auto& l : rankvec[k]) {
            if ((rand() % 100) < p_in * 100) {
              add_edge(j, l);
            }
          }
        }
      }
    }
    this->Light_prune();
    return;
  }

  void add_edge(int a, int b) {
    N[b].parents.push_back(a);
  }

  void Light_prune() {
    for (int i = 0; i < N.size(); i++) {
      for (auto& p : N[i].parents) {
        std::set<int> checkc = N[p].ancestors;
        N[i].ancestors.merge(checkc);
      }
      std::vector<int> newparents;
      for (int j = 0; j < N[i].parents.size(); j++) {
        if (!N[i].ancestors.count(N[i].parents[j])) {
          newparents.push_back(N[i].parents[j]);
          N[i].ancestors.insert(N[i].parents[j]);
        }
      }
      N[i].parents = newparents;
    }
    return;
  }

  void FileOutput(fs::path& p, bool output = true) {
    // output = true => Adjecent
    // output = false => table // TODO
    std::ofstream output_file;
    output_file.open(p);
    if (output) {
      output_file << N.size() << std::endl;
      for (auto& i : N) {
        output_file << i.parents.size() << std::endl;
        for (int j = 0; j < i.parents.size(); j++) {
          output_file << i.parents[j];
          if (j != i.parents.size() - 1) {
            output_file << " ";
          }
          else {
            output_file << std::endl;
          }
        }
      }
    }
    else {
    }
    output_file.close();
  }
};

} // namespace MDG