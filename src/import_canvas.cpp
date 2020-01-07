#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "em_import.h"

using namespace emscripten;

// Declarations

EM_IMPORT_STRUCT("Uint8Array", Uint8Array, {
  EM_IMPORT_FIELD_GETTER("length", int, length);
  // ??? need to figure out how to map this to sequences, might just be a
  // special-case builtin
  // int operator[](int, int);
  void set(int, unsigned char);
});

EM_IMPORT_STRUCT("ImageData", ImageData, {
  EM_IMPORT_CONSTRUCTOR ImageData(double, double);
  EM_IMPORT_FIELD_GETTER("data", Uint8Array, data);
  EM_IMPORT_FIELD_GETTER("width", int, width);
  EM_IMPORT_FIELD_GETTER("height", int, height);
});

typedef double foo; // TODO: This should resolve to "double" in the bindings
EM_IMPORT_STRUCT("CanvasRenderingContext2D", CanvasRenderingContext2D, {
  EM_IMPORT_METHOD("clearRect") void clearRect(double, double, double, double);

  EM_IMPORT_FIELD_SETTER("fillStyle", char*, fillStyle);
  EM_IMPORT_METHOD("fillRect") void fillRect(double, double, double, double);

  EM_IMPORT_FIELD_SETTER("font", char*, font);
  EM_IMPORT_METHOD("fillText") void fillText(char*, double, double);

  EM_IMPORT_METHOD("save") void save();
  EM_IMPORT_METHOD("restore") void restore();
  EM_IMPORT_METHOD("translate") void translate(double, double);

  EM_IMPORT_METHOD("beginPath") void beginPath();
  EM_IMPORT_METHOD("moveTo") void moveTo(foo, double);
  EM_IMPORT_METHOD("lineTo") void lineTo(double, foo);
  EM_IMPORT_METHOD("stroke") void stroke();

  EM_IMPORT_METHOD("putImageData") void putImageData(ImageData, double, double);
});

EM_IMPORT_STRUCT("HTMLCanvasElement", HTMLCanvasElement, {
  EM_IMPORT_METHOD("getContext") CanvasRenderingContext2D getContext(char*);
});

EM_IMPORT_STRUCT("Document", DocumentElement, {
  EM_IMPORT_METHOD("getElementById") HTMLCanvasElement getElementById(char*);
});

EM_IMPORT_STRUCT("Performance", Performance, {
  EM_IMPORT_METHOD("now") double now();
});

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
  auto image = ImageData(500, 200);
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
