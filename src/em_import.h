#pragma once

namespace emscripten {

struct JSObject {
private: struct Impl; Impl* impl;
};

// EM_IMPORT_STRUCT: declares an external type and methods defined on it
#define EM_IMPORT_STRUCT(importName, cppName, body) \
  struct cppName : public JSObject body;

#define EM_IMPORT_METHOD(importName, retTy, name, ...) \
  retTy name(__VA_ARGS__);

#define EM_IMPORT_FIELD_GETTER(importName, ty, name) \
  ty get_ ## name();
#define EM_IMPORT_FIELD_SETTER(importName, ty, name) \
  void set_ ## name(ty);
#define EM_IMPORT_FIELD(importName, ty, name) \
  EM_IMPORT_FIELD_GETTER(importName, ty, name); \
  EM_IMPORT_FIELD_SETTER(importName, ty, name);

extern JSObject getGlobal(char*);

template <typename T> T upcast(JSObject &&obj) {
  return *(T*)(void*)&obj;
}
template <typename T> T getGlobal(char* name) {
  return upcast<T>(getGlobal(name));
}

} // namespace emscripten
