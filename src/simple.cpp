#include <emscripten/bind.h>

int myConst = 12;

int fact(int n) {
  int result = 1;
  for (int i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
}

using namespace emscripten;

EMSCRIPTEN_BINDINGS(my_module) {
  function("fact", &fact);

  constant("myConst", myConst);
}
