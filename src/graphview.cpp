#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "../include/FileUtil.hpp"
#include "../include/Params.hpp"
namespace fs = std::filesystem;

const std::string Graphvis_pref = "https://dreampuf.github.io/GraphvizOnline/#";
const bool FileOutput           = false;
const bool OpenInBrowser        = true;

int main(int argc, char* argv[]) {
  std::string url = Graphvis_pref;
  std::vector<std::string> Outputs;
  if (argc == 2) {
    std::cout << "Input from File :" << argv[2] << std::endl;
    std::string paths = argv[2];
    fs::path p        = paths;
    std::ifstream File;
    File.open(p);
    std::string inString;
    while (std::getline(File, inString)) {
      Outputs.push_back(inString);
    }
  }
  else if (argc == 1) {
    std::cout << "Input from SI" << std::endl;
    int N;
    std::cin >> N;
    Outputs.push_back(std::to_string(N));
    for (int i = 0; i < N; i++) {
      int parnm;
      std::cin >> parnm;
      Outputs.push_back(std::to_string(parnm));
      if (parnm == 0) {
        continue;
      }
      else {
        std::string Sin;
        for (int j = 0; j < parnm; j++) {
          int k;
          std::cin >> k;
          Sin += std::to_string(k);
          if (j != parnm - 1) {
            Sin += " ";
          }
        }
        Outputs.push_back(Sin);
      }
    }
  }
  else {
    std::cout << "invalid arguments" << std::endl;
    return 0;
  }
  MDG::FileUtil FU;
  fs::path Dirp;
  if (FileOutput) {
    Dirp          = FU.CreateDirectory(MDG::Output_directory, "Graph");
    auto Inputtxt = FU.CreateTxtFile(Dirp, "inputgraph");
    std::ofstream ofsinp;
    ofsinp.open(Inputtxt);
    for (auto& i : Outputs) {
      ofsinp << i << std::endl;
    }
    ofsinp.close();
  }
  std::vector<std::string> Graphvtxt;
  Graphvtxt.push_back("digraph G{");

  int ii = 0;
  int jj = 1;
  while (jj < Outputs.size()) {
    Graphvtxt.push_back(std::to_string(ii) + " [label=" + std::to_string(ii) +
                        "];");
    if (Outputs[jj] == "0") {
      jj++;
      ii++;
      continue;
    }
    else {
      int Edgenum = stoi(Outputs[jj]);
      jj++;
      std::istringstream iss(Outputs[jj]);
      std::string s;
      for (int k = 0; k < Edgenum; k++) {
        iss >> s;
        Graphvtxt.push_back(s + " -> " + std::to_string(ii) + " ;");
      }
      jj++;
      ii++;
    }
  }
  Graphvtxt.push_back("}");

  if (FileOutput) {
    auto Outputtxt = FU.CreateTxtFile(Dirp, "Outputgraph");
    std::ofstream ofs;
    ofs.open(Outputtxt);
    for (auto& i : Graphvtxt) {
      ofs << i << std::endl;
    }
  }
  if (OpenInBrowser) {
    for (auto& i : Graphvtxt) {
      url += i;
    }
    std::string command =
        "rundll32.exe url.dll,FileProtocolHandler \"" + url + "\"";
    std::cout << url;
    // system(command.c_str());
  }
}