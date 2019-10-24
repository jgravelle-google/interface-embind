// #include "em_import.h"
#include <functional>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Declarations

// ideal API
// EM_IMPORT_STRUCT: declares an external type and methods defined on it
/*
EM_IMPORT_STRUCT("CanvasRenderingContext2D", CanvasRenderingContext2D, {
  EM_IMPORT_METHOD("fillRect", void, fillRect, double, double, double, double);
  EM_IMPORT_FIELD("fillStyle", std::string, fillStyle);
});
EM_IMPORT_STRUCT("HTMLCanvasElement", HTMLCanvasElement, {
  EM_IMPORT_METHOD("getContext", CanvasRenderingContext2D, getContext, std::string);
});
EM_IMPORT_STRUCT("Document", DocumentElement, {
  EM_IMPORT_METHOD("getElementById", HTMLCanvasElement, getElementById, std::string);
});

DocumentElement document = EM_IMPORT_GLOBAL(DocumentElement, "document");
*/

// hacked API
// template <typename T>
// struct JSField {
//   JSField(std::function<T&> f) : accessor(f) {}
//   operator T&() {
//     return accessor();
//   }
//   JSField operator=(T& val) {
//     accessor() = val;
//     return *this;
//   }
// private:
//   std::function<T&> accessor;
// };
// template <typename T>
// JSField<T> jsField(T& val) {
//   auto f = [&]() { return val; };
//   return JSField<T>(f);
// }

class JSObject {
  struct Impl; Impl* impl;
public:
  template <typename T>
  class Field {
    // JSObject* self;
    // using Get = T(JSObject::*)();
    // using Set = void(JSObject::*)(T);
    using Get = std::function<T()>;
    using Set = std::function<void(T)>;
    Get getter;
    Set setter;
  public:
    Field(JSObject* obj, Get get, Set set)
        : getter(get), setter(set) {}
    operator T() {
      return getter();
    }
    Field operator=(T val) {
      setter(val);
      return *this;
    }
  };
};
struct Uint8Array : public JSObject {
  Uint8Array() :
    length(this,
      [&](){ return get_length(); },
      nullptr)
  {}
  Field<int> length;
  int get_length();
  // int operator[](int, int);
  void set(int, unsigned char);
};
struct ImageData : public JSObject {
  ImageData() :
    width(this, [&]() { return get_width(); }, nullptr),
    height(this, [&]() { return get_height(); }, nullptr)
  {}
  Field<int> width;
  Field<int> height;
  int get_width();
  int get_height();
  static ImageData new_(double, double);
  Uint8Array get_data();
};
struct CanvasRenderingContext2D: public JSObject {
  void clearRect(double, double, double, double);

  void set_fillStyle(char*);
  void fillRect(double, double, double, double);

  void set_font(char*);
  void fillText(char*, double, double);

  void save();
  void restore();
  void translate(double, double);

  void beginPath();
  void moveTo(double, double);
  void lineTo(double, double);
  void stroke();

  void putImageData(ImageData, double, double);
};
struct HTMLCanvasElement: public JSObject {
  CanvasRenderingContext2D getContext(char*);
};
struct DocumentElement: public JSObject {
  HTMLCanvasElement getElementById(char*);
};
struct Performance: public JSObject {
  double now();
};

extern DocumentElement getDocument();
extern JSObject getGlobal(char*);

template <typename T>
T upcast(JSObject &&obj) {
  return *(T*)(void*)&obj;
}
template <typename T>
T getGlobal(char* name) {
  return upcast<T>(getGlobal(name));
}

// Body

CanvasRenderingContext2D ctx;
Performance performance;
double lastTime;
double t = 0.0;

__attribute__((used))
void init() {
  auto document = getGlobal<DocumentElement>("document");
  HTMLCanvasElement canvas = document.getElementById("canvas");
  ctx = canvas.getContext("2d");

  performance = getGlobal<Performance>("performance");
  lastTime = performance.now();
}

void rects(CanvasRenderingContext2D &ctx) {
  ctx.set_fillStyle("rgb(200, 0, 0)");
  ctx.fillRect(10, 10, 200, 200);
  ctx.set_fillStyle("#11cc22");
  ctx.fillRect(40, 40, 160, 160);
  ctx.set_fillStyle("blue");
  ctx.fillRect(70, 50, 120, 120);
}

void sines(CanvasRenderingContext2D &ctx) {
  double x0 = 50.0;
  double y0 = 400.0;
  double sineWidth = 700.0;
  double sineHeight = 300.0;
  double segments = 256;
  double lines = 7;
  double freq = 2;
  ctx.save();
  ctx.translate(x0, y0);
  // todo: translate and declare from here
  double tau = 2.0 * 3.1415926535;
  for (double j = 1; j <= lines; j += 1.0) {
    double off = j / lines * t * tau / 3;
    double f = freq + 0.3 * cos(off / 5.1);
    ctx.beginPath();
    ctx.moveTo(0, sineHeight / 2 * sin(off));
    for (double i = 1; i < segments; i += 1.0) {
      double x = i * sineWidth / segments;
      double y = sineHeight / 2 * sin(i * f / segments * tau + off);
      ctx.lineTo(x, y);
    }
    ctx.stroke();
  }
  ctx.restore();
}

void naiveImage(CanvasRenderingContext2D &ctx) {
  auto image = ImageData::new_(500, 200);
  Uint8Array data = image.get_data();
  int len = data.get_length();
  int imgWidth = image.get_width();
  int imgHeight = image.get_height();
  for (int i = 0; i < len; i += 4) {
    int p = i / 4;
    double x = p % imgWidth;
    double y = (double)p / imgWidth;
    double u = (double)x / imgWidth;
    double v = (double)y / imgHeight;
    data.set(i + 0, 255 * abs(sin(t)) * u);
    data.set(i + 1, 255 * abs(cos(t)) * v);
    data.set(i + 2, 255 * (1.0 - u * v));
    data.set(i + 3, 255);
    // data[i + 0] = 255 * abs(sin(t)) * u;
    // data[i + 1] = 255 * abs(cos(t)) * v;
    // data[i + 2] = 255 * (1.0 - u * v);
    // data[i + 3] = 255;
  }
  ctx.putImageData(image, 260, 10);
}

void framerate(CanvasRenderingContext2D &ctx, double dT) {
  ctx.set_font("20px sans");
  ctx.set_fillStyle("#444444");
  char *msg = (char*)alloca(256);
  int us = 1000.0 * dT;
  snprintf(msg, 256, "Frame time: %dus", us);
  ctx.fillText(msg, 560, 585);
}

__attribute__((used))
void frame() {
  double startTime = performance.now();
  double dT = (startTime - lastTime) / 1000.0;
  lastTime = startTime;
  t += dT;

  ctx.clearRect(0, 0, 800, 600);

  rects(ctx);
  sines(ctx);
  naiveImage(ctx);

  double endTime = performance.now();
  framerate(ctx, endTime - startTime);
}
