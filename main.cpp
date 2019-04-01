#include "driver.h"
#include "parser.tab.hh"
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, const char *argv[]) {
  std::ios::sync_with_stdio(false);
  auto loc = std::locale(std::locale(""), "C", std::locale::numeric);
  std::locale::global(loc);
  std::wcin.imbue(std::locale());
  std::wcout.imbue(std::locale());
  std::wcerr.imbue(std::locale());
  std::wstring line;
  DriverState st;
  std::vector<const char *> args(argv + 1, argv + argc);
  std::wistream *in = &std::wcin;
  bool fileInput = false;
  if (args.size() == 1) {
    in = new std::wifstream(args[0]);
    fileInput = true;
  }
  while (!std::getline(*in, line).eof()) {
    if (fileInput)
      std::wcerr << "Input: " << line << std::endl;
    Driver drv(st, line);
    yy::parser parser(drv);
    // parser.set_debug_level(true);
    try {
      auto res = parser.parse();
      if (res != 0) {
        std::wcerr << L"Parser returned error " << res << std::endl;
      } else {
        if (drv.result)
          std::wcout << "Result is: " << drv.result.value() << std::endl;
      }
    } catch (std::exception &e) {
      std::wcerr << e.what() << std::endl;
    }
  }
  if (fileInput) {
    delete in;
  }
  return 0;
}
