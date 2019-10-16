#include <emscripten/bind.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

using namespace emscripten;

val ctx = val::undefined();
double lastTime;
double t = 0.0;

void init() {
  val document = val::global("document");
  val canvas = document.call<val>("getElementById", val("canvas"));
  ctx = canvas.call<val>("getContext", val("2d"));

  lastTime = val::global("performance").call<double>("now");
}

void rects(val ctx) {
  ctx.set("fillStyle", val("rgb(200, 0, 0)"));
  ctx.call<void>("fillRect", 10, 10, 200, 200);
  ctx.set("fillStyle", val("#11cc22"));
  ctx.call<void>("fillRect", 40, 40, 160, 160);
  ctx.set("fillStyle", val("blue"));
  ctx.call<void>("fillRect", 70, 50, 120, 120);
}

void sines(val ctx) {
  double x0 = 50.0;
  double y0 = 400.0;
  double sineWidth = 700.0;
  double sineHeight = 300.0;
  double segments = 256;
  double lines = 7;
  double freq = 2;
  ctx.call<void>("save");
  ctx.call<void>("translate", x0, y0);
  double tau = 2.0 * 3.1415926535;
  for (double j = 1; j <= lines; j += 1.0) {
    double off = j / lines * t * tau / 3;
    double f = freq + 0.3 * cos(off / 5.1);
    ctx.call<void>("beginPath");
    ctx.call<void>("moveTo", 0, sineHeight / 2 * sin(off));
    for (double i = 1; i < segments; i += 1.0) {
      double x = i * sineWidth / segments;
      double y = sineHeight / 2 * sin(i * f / segments * tau + off);
      ctx.call<void>("lineTo", x, y);
    }
    ctx.call<void>("stroke");
  }
  ctx.call<void>("restore");
}

void naiveImage(val ctx) {
  // NOTE: very slow
  val image = val::global("ImageData").new_(500, 200);
  int len = image["data"]["length"].as<int>();
  int imgWidth = image["width"].as<int>();
  int imgHeight = image["height"].as<int>();
  for (int i = 0; i < len; i += 4) {
    int p = i / 4;
    double x = p % imgWidth;
    double y = (double)p / imgWidth;
    double u = (double)x / imgWidth;
    double v = (double)y / imgHeight;
    image["data"].set(i + 0, 255 * abs(sin(t)) * u);
    image["data"].set(i + 1, 255 * abs(cos(t)) * v);
    image["data"].set(i + 2, 255 * (1.0 - u * v));
    image["data"].set(i + 3, 255);
  }
  ctx.call<void>("putImageData", image, 260, 10);
}

void framerate(val ctx, double dT) {
  ctx.set("font", val("20px sans"));
  ctx.set("fillStyle", val("#444444"));
  char *msg = (char*)alloca(256);
  int us = 1000.0 * dT;
  snprintf(msg, 256, "Frame time: %dus", us);
  ctx.call<void>("fillText", val(std::string(msg)), 560, 585);
  // free(msg);
}

void frame() {
  double startTime = val::global("performance").call<double>("now");
  double dT = (startTime - lastTime) / 1000.0;
  lastTime = startTime;
  t += dT;

  ctx.call<void>("clearRect", 0, 0, 800, 600);

  rects(ctx);
  sines(ctx);
  // naiveImage(ctx);
  double endTime = val::global("performance").call<double>("now");
  framerate(ctx, endTime - startTime);
}

EMSCRIPTEN_BINDINGS(my_module) {
  function("init", &init);
  function("frame", &frame);
}
