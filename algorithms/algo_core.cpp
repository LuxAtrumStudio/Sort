#include <math.h>
#include <map>
#include <string>
#include <vector>
#include "../sorting/sort.hpp"
#include "algo_core.hpp"
#include "algorithm_headers.hpp"

namespace sort {
  AlgoResult result;
  std::map<std::string, void (*)()> algorithms;
  std::vector<int> data_base, data;
}

void sort::LoadAlgos() { algorithms["quicksort"] = Quicksort; }

bool sort::RunAlgo(std::string algo) {
  std::map<std::string, void (*)()>::iterator it;
  it = algorithms.find(algo);
  bool good = false;
  if (it != algorithms.end()) {
    win.Print("Running %s...\n", algo.c_str());
    ClearResults();
    data = data_base;
    it->second();
    win.Print("Compleated %s\n", algo.c_str());
    PrintResults();
    good = true;
  } else {
    win.Print("%s is not a valid sorting algorithm\n", algo.c_str());
    good = false;
  }
  return (good);
}

void sort::PrintResults() {
  double dmin, dsec, dmilli_sec, dmicro_sec;
  double remaining_time = result.time_elapsed;
  dmin = floor(result.time_elapsed / 60.0);
  remaining_time = modf(remaining_time, &dsec);
  dsec -= (dmin * 60);
  remaining_time *= pow(10, 3);
  remaining_time = modf(remaining_time, &dmilli_sec);
  remaining_time *= pow(10, 3);
  dmicro_sec = ceil(remaining_time);
  int min = dmin, sec = dsec, milli_sec = dmilli_sec,
      micro_sec = (int)dmicro_sec;
  win.Print(
      "[RESULTS]\nSort Time: %fs\nFormated Time: "
      "%.2im:%.2is:%.3ims:%.3iµs\nComparisons: "
      "%li\nArray "
      "Access: "
      "%li\n",
      result.time_elapsed, min, sec, milli_sec, micro_sec, result.comparisons,
      result.vec_access);
}
void sort::ClearResults() {
  result.time_elapsed = 0;
  result.comparisons = 0;
  result.vec_access = 0;
}

void sort::GenData(std::string settings) {
  std::string min_str = "", max_str = "", count_str = "";
  std::string sub_string = "";
  int min = 0, max = 100000;
  int count = 10000;
  for (int i = 0; i < settings.length(); i++) {
    if (settings[i] == ' ' && sub_string != "") {
      if (count_str == "") {
        count_str = sub_string;
      } else if (max_str == "") {
        max_str = sub_string;
      } else if (min_str == "") {
        min_str = sub_string;
      }
      sub_string = "";
    } else if (settings[i] == ' ') {
      sub_string = "";
    } else {
      sub_string += settings[i];
    }
  }
  if (sub_string != "") {
    if (count_str == "") {
      count_str = sub_string;
    } else if (max_str == "") {
      max_str = sub_string;
    } else if (min_str == "") {
      min_str = sub_string;
    }
  }
  if (count_str.length() != 0 && IsInt(count_str) != -1) {
    count = IsInt(count_str);
  }
  if (max_str.length() != 0 && IsInt(max_str) != -1) {
    max = IsInt(max_str);
  }
  if (min_str.length() != 0 && IsInt(min_str) != -1) {
    min = IsInt(min_str);
  }
  int range = max - min;
  win.Print("Generating Data...\n%i values between:\n(%i-%i)\n", count, min,
            max);
  data_base.clear();
  for (long int i = 0; i < count; i++) {
    data_base.push_back(min + (rand() % range));
  }
}
