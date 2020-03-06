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

test:
	ninja -C /s/emscripten/tools/em-import/build/
	make out/hello_em_import.wasm
	node out/hello_em_import.js
.PHONY: test

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
	emcc src/import_canvas.cpp -o out/import_canvas.o --profiling-funcs -O2 \
		-s ERROR_ON_UNDEFINED_SYMBOLS=0 -std=c++11 -Wno-writable-strings \
		-I../em-sys/out/ -Isrc/
	emcc out/import_canvas.o -o out/import_canvas.js --profiling-funcs -O1 \
		-s ERROR_ON_UNDEFINED_SYMBOLS=0 -std=c++11 -Wno-writable-strings
# 	/s/wbin/wasm-dis out/import_canvas.o -o out/import_canvas.o.wat
# 	/s/wbin/wasm-dis out/import_canvas.wasm -o out/import_canvas.wat
	/s/wbin/wasm-decompile out/import_canvas.wasm -o out/import_canvas.wade

out/hello_em_import.wasm: src/hello_em_import.cpp out/
	emcc src/hello_em_import.cpp -o out/hello_em_import.js --profiling-funcs -O1 \
		-s ERROR_ON_UNDEFINED_SYMBOLS=0 -std=c++11 -Wno-writable-strings \
		-I../em-sys/out/ -Isrc/

out/import_canvas_loader.js: src/import_canvas.js
	cp src/import_canvas.js out/import_canvas_loader.js

out/import_canvas.html: out/import_canvas.wasm out/import_canvas_loader.js \
		src/import_canvas.html src/navbar.html
	cat src/import_canvas.html src/navbar.html > out/import_canvas.html

out/test.wasm: src/a.cpp src/b.cpp out/
	emcc src/a.cpp src/b.cpp -o out/test.wasm
	wasm-objdump out/test.wasm -h -s -j em-import
