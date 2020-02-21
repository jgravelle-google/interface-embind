#include "em_import.h"

using namespace emscripten;

EM_IMPORT_FUNC("console.log") void log(const char* msg);

int main() {
  log("Hi there worldio");
  return 0;
}
