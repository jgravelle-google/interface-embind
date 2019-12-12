__attribute__((annotate("EM_IMPORT")))
int attr_lo() { return 10; }
__attribute__((annotate("EM_IMPORT")))
int attr_hi() { return 100; }


__attribute__((annotate("EM_IMPORT")))
int f_a(int x) {
  return 2 * x;
}

struct S {
__attribute__((annotate("EM_IMPORT")))
int g_a(int x);
__attribute__((annotate("EM_IMPORT")))
int g_b(int x);
};

void foo() {
  S x;
  x.g_a(1);
  x.g_b(1);
}
