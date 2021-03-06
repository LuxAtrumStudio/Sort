#include "sort.hpp"
#include <ncurses.h>
#include <ostendo.h>
#include <pessum.h>
#include <fstream>
#include <iostream>
#include <string>
#include "../algorithms/algo_core.hpp"

namespace sort {
  bool running = true;
  std::vector<std::string> help_doc;
  ostendo::Window win(10, 10, 10, 10);
}

using namespace ostendo;

void sort::MainLoop() {
  running = true;
  win.NewWindow(ostendo::std_scr.w, ostendo::std_scr.h, 0, 0);
  win.ToggleTitle("Sorting Algorithms");
  win.ToggleBorder();
  win.ToggleScroll();
  PrintOpen();
  LoadHelp();
  LoadAlgos();
  GenData("");
  std::vector<std::string> past_input;
  int input_index = past_input.size();
  std::string input = "";
  bool update = true;
  while (running == true) {
    if (update == true) {
      update = false;
      win.Print(">>%s", input.c_str());
    }
    int in = getch();
    if (in == 10) {
      win.Print("\n");
      RunInput(input);
      past_input.push_back(input);
      input = "";
      input_index = past_input.size();
      update = true;
    } else if (in == KEY_BACKSPACE) {
      if (input.length() > 0) {
        win.ClearLine();
        input.pop_back();
        update = true;
      }
    } else if (in == KEY_UP) {
      if (input_index > 0) {
        input_index--;
        input = past_input[input_index];
        win.ClearLine();
        update = true;
      }
    } else if (in == KEY_DOWN) {
      if (input_index < past_input.size() - 1) {
        input_index++;
        input = past_input[input_index];
        win.ClearLine();
        update = true;
      } else if (input_index == past_input.size() - 1) {
        input_index++;
        input = "";
        win.ClearLine();
        update = true;
      }
    } else if (in != ERR) {
      input += char(in);
      win.SetCurs(-1, 1);
      update = true;
    }
  }
  win.DelWin();
}

void sort::PrintOpen() {
  win.Print(
      "==========SORTING ALGORITHMS==========\n"
      "Type \"quit\" to quit\n"
      "Type \"help\" for help page\n\n");
}

void sort::Help(int page) {
  if (page < help_doc.size()) {
    std::string page_str = help_doc[page];
    win.Clear();
    win.Print("==========HELP PAGE %i==========\n%s", page, page_str.c_str());
  } else {
    win.Print("No Help Page %i Exists\n", page);
  }
}

void sort::LoadHelp() {
  std::ifstream help_file("help.txt");
  if (help_file.is_open()) {
    std::string line, page = "";
    while (getline(help_file, line)) {
      if (line == "/newpage") {
        help_doc.push_back(page);
        page = "";
      } else {
        page += line + "\n";
      }
    }
    help_doc.push_back(page);
  } else {
    win.Print("[ERR] : NO HELP FILE FOUND\n");
  }
}

void sort::RunInput(std::string input) {
  if (input == "quit") {
    running = false;
  } else if (input[0] == 'h' && input[1] == 'e' && input[2] == 'l' &&
             input[3] == 'p') {
    input.erase(input.begin(), input.begin() + 4);
    if (input.length() != 0) {
      Help(IsInt(input));
    } else {
      Help(0);
    }
  } else if (input == "clear") {
    win.Clear();
  } else if (input[0] == 'd' && input[1] == 'a' && input[2] == 't' &&
             input[3] == 'a') {
    input.erase(input.begin(), input.begin() + 4);
    GenData(input);
  } else if (input == "check") {
    if (IsSorted() == true) {
      win.Print("The data set was successfuly sorted\n");
    } else {
      win.Print("The data set was not successfuly sorted\n");
    }
    if (data.size() < data_base.size()) {
      win.Print("Data was lost durring sorting%i->%i\n", data_base.size(),
                data.size());
    } else if (data.size() > data_base.size()) {
      win.Print("Data was gained durring sorting%i->%i\n", data_base.size(),
                data.size());
    }
  } else if (input == "size") {
    win.Print("Rows:%i\nCols:%i\n", std_scr.h, std_scr.w);
  } else if (input == "print") {
    for (int i = 0; i < data.size(); i++) {
      win.Print("%i,", data[i]);
    }
    win.Print("\n");
  } else if (input == "list") {
    for (std::map<std::string, void (*)()>::iterator it = algorithms.begin();
         it != algorithms.end(); ++it) {
      win.Print("%s\n", it->first.c_str());
    }
  } else if(input == "save"){
    SaveData();
  }else {
    std::vector<std::string> algos;
    std::string sub_string = "";
    for (int i = 0; i < input.size(); i++) {
      if (input[i] == ' ') {
        algos.push_back(sub_string);
        sub_string = "";
      } else {
        sub_string += input[i];
      }
    }
    algos.push_back(sub_string);
    Run(algos);
  }
}

int sort::IsInt(std::string str) {
  bool good = true;
  for (int i = 0; i < str.length(); i++) {
    if ((int(str[i]) > 57 || int(str[i]) < 48) && str[i] != ' ') {
      good = false;
    }
  }
  int str_int = -1;
  if (good == true) {
    str_int = stoi(str);
  }
  return (str_int);
}

std::string sort::NormalizeInput(std::string input) {
  for (int i = 0; i < input.length(); i++) {
    input[i] = tolower(input[i]);
  }
  return (input);
}
