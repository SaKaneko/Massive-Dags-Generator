#include "../include/Params.hpp"
#include "../include/GraphGenerator.hpp"
#include "../include/FileUtil.hpp"
#include <iostream>
#include <vector>
#include <string>
namespace fs = std::filesystem;
MDG::FileUtil FU;

int main(int argc, char* argv[]) {
  fs::path thispath;
  if(argc==2){
    thispath = argv[1];
  }else{
    thispath = MDG::Output_directory;
  }
  thispath          = FU.CreateDirectory(thispath, MDG::prefix);

  for (auto& i : MDG::nodes_loader) {
    MDG::nodes = i;
    fs::path nodespath =
        FU.CreateDirectory(thispath, "nodes_" + std::to_string(i));
    for (auto& j : MDG::alpha_loader) {
      MDG::alpha = j;
      fs::path alphapath =
          FU.CreateDirectory(nodespath, "alpha_" + std::to_string(j));
      for (auto& k : MDG::p_loader) {
        MDG::p = k;
        fs::path ppath =
            FU.CreateDirectory(alphapath, "p_" + std::to_string(k));
        for (int l = 0; l < MDG::iteration; l++) {
          MDG::Graph G;

          G.GenerateRandomGraphfromParam(i, j, k);
          std::string S = "inputgraph";

          fs::path outputfile = FU.CreateTxtFile(ppath, S);
          MDG::add_conf(ppath);

          G.FileOutput(outputfile);
        }
      }
    }
  }
  /*
  MDG::Graph G;
  G.GenerateRandomGraphfromParam(20, 0.5, 0.75);
  std::string S = std::to_string(20) + "_" + std::to_string(0.5) + "_" +
                          std::to_string(0.75);
  auto pt = FU.CreateTxtFile(".", S);
  G.FileOutput(pt);
  */
}