#pragma once
#include <cstdio>
#include <fstream>
#include <cassert>
#include <filesystem>
#include <string>
#include <set>
#include <map>

namespace fs = std::filesystem;

namespace MDG {
class FileUtil {
  std::map<fs::path, std::map<std::string, int>> rollingmap;

  fs::path RollingNumber(fs::path& p, std::string& FileName,
                         bool direct = false, std::string pref = "_",
                         std::string afterf = "") {
    // This function can be used for only "clean" directory.

    if (!rollingmap.count(p)) {
      std::map<std::string, int> newmp;
      rollingmap[p] = newmp;
    }

    int rollingnumber;
    if (!rollingmap[p].count(FileName)) {
      rollingmap[p][FileName] = 0;
      rollingnumber           = 0;
      if (direct) {
        if (!fs::exists(p / FileName)) {
          rollingmap[p][FileName] = 1;
          return p / FileName;
        }
      }
    }
    else {
      rollingnumber = rollingmap[p][FileName];
      if (!fs::exists(p / FileName)) {
        return p / FileName;
      }
    }

    while (1) {
      if (!fs::exists(
              p / (FileName + pref + std::to_string(rollingnumber) + afterf))) {
        break;
      }
      else {
        rollingnumber++;
      }
    }
    rollingmap[p][FileName] = rollingnumber + 1;
    return p / (FileName + pref + std::to_string(rollingnumber) + afterf);
  }

public:
  fs::path CreateDirectory(fs::path p, std::string DirectoryName) {
    auto getp   = RollingNumber(p, DirectoryName, true);
    auto result = fs::create_directory(getp);
    assert(result);
    return getp;
  }
  fs::path CreateTxtFile(fs::path p, std::string Filename) {
    auto getp = RollingNumber(p, Filename, false, "_", ".txt");
    std::ofstream creating_file;
    creating_file.open(getp);
    creating_file.close();
    return getp;
  }
};
} // namespace MDG