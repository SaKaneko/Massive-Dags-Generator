#pragma once
#include <iostream>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <vector>
#include <set>
#include <queue>
#include <map>
namespace fs = std::filesystem;

namespace Cp {
const int potrft = 0;
const int trsmt  = 1;
const int gemmt  = 2;
const int syrkt  = 3;
} // namespace Cp

namespace MDG {
struct NodeC {
  int type;
  std::vector<int> parent;
  NodeC(int g) : type(g) {}
  void print() {
    std::cout << type << std::endl;
    for (auto& x : parent) {
      std::cout << x << " ";
    }
    std::cout << std::endl;
  }
};
struct Tree {};

void Cholesky_generator(int T, fs::path p) {
  int total = T + T * (T - 1) + T * (T - 1) * (T - 2) / 6;
  std::ofstream ofs;
  ofs.open(p);
  ofs << total << std::endl;
  std::vector<NodeC> nodev;
  std::map<int, int> potrfmap;
  std::map<std::pair<int, int>, int> trsmmap, syrkmap;
  std::vector<std::map<std::pair<int, int>, int>> gemmmap(T);
  NodeC insnode(0);
  for (int i = 0; i < T; i++) {
    insnode.type = Cp::potrft;
    nodev.push_back(insnode);
    potrfmap[i] = nodev.size() - 1;
    for (int j = i + 1; j < T; j++) {
      insnode.type = Cp::trsmt;
      nodev.push_back(insnode);
      trsmmap[{i, j}] = nodev.size() - 1;
    }
    for (int j = i + 1; j < T; j++) {
      insnode.type = Cp::syrkt;
      nodev.push_back(insnode);
      syrkmap[{i, j}] = nodev.size() - 1;
      for (int k = j + 1; k < T; k++) {
        insnode.type = Cp::gemmt;
        nodev.push_back(insnode);
        gemmmap[i][{j, k}] = nodev.size() - 1;
      }
    }
  }

  // R1
  for (int i = 0; i < T; i++) {
    for (int j = i + 1; j < T; j++) {
      nodev[trsmmap[{i, j}]].parent.push_back(potrfmap[i]);
    }
  }
  // R2
  for (int i = 0; i < T; i++) {
    for (int j = i + 1; j < T; j++) {
      nodev[syrkmap[{i, j}]].parent.push_back(trsmmap[{i, j}]);
    }
  }
  // R3
  for (int j = 0; j < T; j++) {
    for (int k = j + 1; k < T; k++) {
      for (int i = k + 1; i < T; i++) {
        if (gemmmap[j].count({k, i}))
          nodev[gemmmap[j][{k, i}]].parent.push_back(trsmmap[{j, i}]);
      }
    }
  }
  // R4
  for (int j = 0; j < T; j++) {
    for (int i = j + 1; i < T; i++) {
      for (int k = i + 1; k < T; k++) {
        if (gemmmap[j].count({i, k}))
          nodev[gemmmap[j][{i, k}]].parent.push_back(trsmmap[{j, i}]);
      }
    }
  }

  // R5
  for (int j = 0; j < T - 1; j++) {
    for (int i = j + 2; i < T; i++) {
      nodev[syrkmap[{j + 1, i}]].parent.push_back(syrkmap[{j, i}]);
    }
  }
  // R6
  for (int j = 1; j < T; j++) {
    nodev[potrfmap[j]].parent.push_back(syrkmap[{j - 1, j}]);
  }
  // R7
  for (int j = 1; j < T; j++) {
    for (int i = j + 1; i < T; i++) {
      nodev[trsmmap[{j, i}]].parent.push_back(gemmmap[j - 1][{j, i}]);
    }
  }
  // R8
  for (int k = 0; k < T - 1; k++) {
    for (int j = k + 2; j < T; j++) {
      for (int i = j + 1; i < T; i++) {
        nodev[gemmmap[k + 1][{j, i}]].parent.push_back(gemmmap[k][{j, i}]);
      }
    }
  }
  for (int i = 0; i < nodev.size(); i++) {
    ofs << nodev[i].parent.size() << " " << nodev[i].type << std::endl;
    if (!nodev[i].parent.empty()) {
      for (int j = 0; j < nodev[i].parent.size(); j++) {
        if (j == nodev[i].parent.size() - 1) {
          ofs << nodev[i].parent[j] << std::endl;
        }
        else {
          ofs << nodev[i].parent[j] << " ";
        }
      }
    }
  }
  ofs.close();
}

} // namespace MDG