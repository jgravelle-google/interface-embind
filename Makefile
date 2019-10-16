all: out/js_canvas.html out/embind_canvas.html

watch:
	mywatch src/* -- make all
.PHONY: watch

out/simple.js: src/simple.cpp src/simple_pre.js
	mkdir -p out/
	emcc src/simple.cpp -o out/simple.js --bind --pre-js src/simple_pre.js

run: out/simple.js
	node out/simple.js
.PHONY: run

out/js_canvas.html: src/js_canvas.js src/js_canvas.html
	mkdir -p out/
	cp src/js_canvas.js out/js_canvas.js
	cp src/js_canvas.html out/js_canvas.html

serve:
	/s/emscripten/emrun --no_browser --port 8080 out/
.PHONY: serve

out/embind_canvas.html: out/embind_canvas.wasm src/embind_canvas.html
	mkdir -p out/
	cp src/embind_canvas.html out/embind_canvas.html

out/embind_canvas.wasm: src/canvas.cpp src/canvas_pre.js
	mkdir -p out/
	emcc src/canvas.cpp -o out/embind_canvas.js --bind --pre-js src/canvas_pre.js --profiling-funcs -O2
# 	/s/wbin/wasm-dis out/embind_canvas.wasm -o out/embind_canvas.wat
# 	/s/wbin/wasm-decompile out/embind_canvas.wasm -o out/embind_canvas.wade
