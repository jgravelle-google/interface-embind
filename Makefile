all: out/js_canvas.html out/embind_canvas.html out/import_canvas.html

watch:
	mywatch src/* -- make all
.PHONY: watch

out/:
	mkdir -p out/

out/simple.js: src/simple.cpp src/simple_pre.js out/
	emcc src/simple.cpp -o out/simple.js --bind --pre-js src/simple_pre.js

run: out/simple.js
	node out/simple.js
.PHONY: run

out/js_canvas.html: src/js_canvas.js src/js_canvas.html src/navbar.html out/
	cp src/js_canvas.js out/js_canvas.js
	cat src/js_canvas.html src/navbar.html > out/js_canvas.html

serve: out/
	/s/emscripten/emrun --no_browser --port 1337 out/
.PHONY: serve

out/embind_canvas.html: out/embind_canvas.wasm src/embind_canvas.html src/navbar.html
	cat src/embind_canvas.html src/navbar.html > out/embind_canvas.html

out/embind_canvas.wasm: src/canvas.cpp src/canvas_pre.js out/
	emcc src/canvas.cpp -o out/embind_canvas.js --bind --pre-js src/canvas_pre.js --profiling-funcs -O2
	/s/wbin/wasm-dis out/embind_canvas.wasm -o out/embind_canvas.wat
# 	/s/wbin/wasm-decompile out/embind_canvas.wasm -o out/embind_canvas.wade

out/import_canvas.wasm: src/import_canvas.cpp out/
	emcc src/import_canvas.cpp -o out/import_canvas.wasm --profiling-funcs -O2 -s ERROR_ON_UNDEFINED_SYMBOLS=0 -std=c++11
	/s/wbin/wasm-dis out/import_canvas.wasm -o out/import_canvas.wat
	/s/wbin/wasm-decompile out/import_canvas.wasm -o out/import_canvas.wade

out/import_canvas.js: src/import_canvas.js
	cp src/import_canvas.js out/import_canvas.js

out/import_canvas.html: out/import_canvas.wasm src/import_canvas.html out/import_canvas.js src/navbar.html
	cat src/import_canvas.html src/navbar.html > out/import_canvas.html
