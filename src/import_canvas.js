"use strict";

async function loadWasm() {
  const refCache = [undefined];
  let u8, i32;

  // JS -> C++
  function refToIdx(val) {
    const idx = refCache.length;
    refCache.push(val);
    return idx;
  }
  function stringToPtr(str) {
    // todo: write string to memory
  }

  // C++ -> JS
  function idxToRef(idx) {
    return refCache[idx];
  }
  function ptrToRef(ptr) {
    const idx = i32[ptr >> 2];
    return idxToRef(idx);
  }
  function ptrToString(ptr) {
    let str = '';
    let c;
    while (c = u8[ptr++]) {
      str += String.fromCharCode(c);
    }
    return str;
  }
  const imports = {
    env: {
      _Z9getGlobalPc(namePtr) {
        const name = ptrToString(namePtr);
        return refToIdx(window[name]);
      },
      _ZN10Uint8Array10get_lengthEv(arrPtr) {
        const arr = ptrToRef(arrPtr);
        return arr.length;
      },
      _ZN10Uint8Array3setEih(arrPtr, idx, val) {
        const arr = ptrToRef(arrPtr);
        arr[idx] = val;
      },
      _ZN11Performance3nowEv(perfPtr) {
        const perf = ptrToRef(perfPtr);
        return perf.now();
      },
      _ZN15DocumentElement14getElementByIdEPc(docPtr, idPtr) {
        const doc = ptrToRef(docPtr);
        const id = ptrToString(idPtr);
        return refToIdx(doc.getElementById(id));
      },
      _ZN17HTMLCanvasElement10getContextEPc(canvasPtr, idPtr) {
        const canvas = ptrToRef(canvasPtr);
        const id = ptrToString(idPtr);
        return refToIdx(canvas.getContext(id));
      },
      _ZN24CanvasRenderingContext2D12putImageDataE9ImageDatadd(ctxPtr, imgIdx, x, y) {
        const ctx = ptrToRef(ctxPtr);
        const img = idxToRef(imgIdx);
        ctx.putImageData(img, x, y);
      },
      _ZN24CanvasRenderingContext2D13set_fillStyleEPc(ctxPtr, strPtr) {
        const ctx = ptrToRef(ctxPtr);
        const str = ptrToString(strPtr);
        ctx.fillStyle = str;
      },
      _ZN24CanvasRenderingContext2D8fillRectEdddd(ctxPtr, x, y, w, h) {
        const ctx = ptrToRef(ctxPtr);
        ctx.fillRect(x, y, w, h);
      },
      _ZN24CanvasRenderingContext2D8fillTextEPcdd(ctxPtr, strPtr, x, y) {
        const ctx = ptrToRef(ctxPtr);
        const str = ptrToString(strPtr);
        ctx.fillText(str, x, y);
      },
      _ZN24CanvasRenderingContext2D8set_fontEPc(ctxPtr, strPtr) {
        const ctx = ptrToRef(ctxPtr);
        const str = ptrToString(strPtr);
        ctx.font = str;
      },
      _ZN24CanvasRenderingContext2D9clearRectEdddd(ctxPtr, x, y, w, h) {
        const ctx = ptrToRef(ctxPtr);
        ctx.clearRect(x, y, w, h);
      },
      _ZN24CanvasRenderingContext2D4saveEv(ctxPtr) {
        const ctx = ptrToRef(ctxPtr);
        ctx.save();
      },
      _ZN24CanvasRenderingContext2D6lineToEdd(ctxPtr, x, y) {
        const ctx = ptrToRef(ctxPtr);
        ctx.lineTo(x, y);
      },
      _ZN24CanvasRenderingContext2D6moveToEdd(ctxPtr, x, y) {
        const ctx = ptrToRef(ctxPtr);
        ctx.moveTo(x, y);
      },
      _ZN24CanvasRenderingContext2D6strokeEv(ctxPtr) {
        const ctx = ptrToRef(ctxPtr);
        ctx.stroke();
      },
      _ZN24CanvasRenderingContext2D7restoreEv(ctxPtr) {
        const ctx = ptrToRef(ctxPtr);
        ctx.restore();
      },
      _ZN24CanvasRenderingContext2D9beginPathEv(ctxPtr) {
        const ctx = ptrToRef(ctxPtr);
        ctx.beginPath();
      },
      _ZN24CanvasRenderingContext2D9translateEdd(ctxPtr, x, y) {
        const ctx = ptrToRef(ctxPtr);
        ctx.translate(x, y);
      },
      _ZN9ImageData9constructEdd(w, h) {
        const image = new ImageData(w, h);
        return refToIdx(image);
      },
      _ZN9ImageData10get_heightEv(imagePtr) {
        const image = ptrToRef(imagePtr);
        return image.height;
      },
      _ZN9ImageData8get_dataEv(imagePtr) {
        const image = ptrToRef(imagePtr);
        return refToIdx(image.data);
      },
      _ZN9ImageData9get_widthEv(imagePtr) {
        const image = ptrToRef(imagePtr);
        return image.width;
      },
    },
  };
  const fetched = await fetch("import_canvas.wasm");
  const bytes = await fetched.arrayBuffer();
  const result = await WebAssembly.instantiate(bytes, imports);
  const wasm = result.instance.exports;
  u8 = new Uint8Array(wasm.memory.buffer);
  i32 = new Int32Array(wasm.memory.buffer);
  wasm.__wasm_call_ctors();
  wasm._Z4initv();

  function frame() {
    wasm._Z5framev();
    requestAnimationFrame(frame);
  }
  frame();
}

loadWasm();
