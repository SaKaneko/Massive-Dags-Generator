#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <set>
#include <queue>
#include <random>
#include "Params.hpp"
namespace fs = std::filesystem;

namespace MDG {
std::vector<std::string> tabs = {
    "#4E79A7", "#A0CBE8", "#F28E2B", "#FFBE7D", "#59A14F", "#8CD17D", "#B6992D",
    "#F1CE63", "#499894", "#86BCB6", "#E15759", "#FF9D9A", "#79706E", "#BAB0AC",
    "#D37295", "#FABFD2", "#B07AA1", "#D4A6C8", "#9D7660", "#D7B5A6"};

std::random_device rd;
std::mt19937 gen(rd());

int random(int low, int high) {
  std::uniform_int_distribution<> dist(low, high);
  return dist(gen);
}

struct Node {
  std::vector<int> parents;
  std::multiset<int> children;
  std::vector<int> childrenCP;
  int type;
};

struct contents {
  int a, b;
  bool bypass;
  bool operator<(const struct contents& other) const {
    if (a == other.a) {
      return b < other.b;
    }
    else {
      return a < other.a;
    }
  }
};
struct Graph {
  std::vector<Node> N;

  void GenerateRandomGraphfromParam(int nodes_in, double alpha_in,
                                    double p_in) {
    N.resize(nodes_in);
    srand(time(NULL));
    double rankmx = sqrt(nodes_in) / alpha_in;
    int ranks     = random(1, rankmx + 1);
    std::vector<std::vector<int>> rankvec(ranks, std::vector<int>(0));

    for (int i = 0; i < nodes_in; i++) {
      int noderank = random(0, ranks - 1);
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
    this->prune();
    return;
  }

  void add_edge(int a, int b) {
    N[a].children.insert(b);
    N[a].childrenCP.push_back(b);
    N[b].parents.push_back(a);
  }

  std::vector<int> search_leaf() {
    std::vector<int> lf;
    for (int i = 0; i < N.size(); i++) {
      if (N[i].children.size() == 0) {
        lf.push_back(i);
      }
    }
    return lf;
  }
  void ClearParents() {
    for (auto& i : N) {
      i.parents.clear();
    }
  }
  void ReGetParents() {
    for (int i = 0; i < N.size(); i++) {
      for (int j = 0; j < N[i].childrenCP.size(); j++) {
        if (N[i].children.count(N[i].childrenCP[j])) {
          N[N[i].childrenCP[j]].parents.push_back(i);
        }
      }
    }
  }

  void prune() {
    std::vector<int> lf = search_leaf();
    std::queue<contents> q;
    std::vector<bool> check(N.size(), true);
    for (int i = 0; i < lf.size(); i++) {
      std::vector<int> lsnode = {lf[i]};
      for (int j = 0; j < N[lf[i]].parents.size(); j++) {
        contents C = {N[lf[i]].parents[j], lf[i], true};
        q.push(C);
      }
    }
    while (!q.empty()) {
      auto [a, b, bp] = q.front();
      q.pop();
      if (!bp) {
        N[a].children.erase(b);
      }
      for (int i = 0; i < N[a].parents.size(); i++) {
        contents c = {N[a].parents[i], b, false};
        q.push(c);
      }
      if (check[a]) {
        check[a] = false;
        for (int i = 0; i < N[a].parents.size(); i++) {
          contents c = {N[a].parents[i], a, true};
          q.push(c);
        }
      }
    }
    this->ClearParents();
    this->ReGetParents();
    return;
  }

  void print_graphv() {
    std::cout << "digraph {" << std::endl;
    for (int i = 0; i < N.size(); i++) {
      std::cout << i << " [label=" << i << ",style = filled,fillcolor = \""
                << tabs[N[i].type % 20] << "\"];" << std::endl;
      for (int j = 0; j < N[i].childrenCP.size(); j++) {
        if (N[i].children.count(N[i].childrenCP[j])) {
          std::cout << i << " -> " << N[i].childrenCP[j] << std::endl;
        }
      }
    }
    std::cout << "}" << std::endl;
  }

  void print_SO() {
    std::cout << N.size() << std::endl;
    for (auto& i : N) {
      std::cout << i.type << " " << i.parents.size() << std::endl;
      for (int j = 0; j < i.parents.size(); j++) {
        std::cout << i.parents[j];
        if (j == i.parents.size() - 1) {
          std::cout << std::endl;
        }
        else {
          std::cout << " ";
        }
      }
    }
  }
  void FileOutput(fs::path& p, bool output = true) {
    // output = true => Adjecent
    // output = false => table
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
void GenerateRandomGraphsPermPalams() {}

} // namespace MDG
  /*
  int main() {
    MDG::Graph G;
    G.GenerateRandomGraphfromParam();
    G.prune();
    G.print_graphv();
    G.print_SO();
  }*/