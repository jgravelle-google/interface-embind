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
        const ret = Document.prototype.getElementById.apply(doc, [id]);
        return refToIdx(ret);
      },
      _ZN17HTMLCanvasElement10getContextEPc(canvasPtr, idPtr) {
        const canvas = ptrToRef(canvasPtr);
        const id = ptrToString(idPtr);
        const ret = HTMLCanvasElement.prototype.getContext.apply(canvas, [id]);
        return refToIdx(ret);
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
        CanvasRenderingContext2D.prototype.fillRect.apply(ctx, [x, y, w, h]);
      },
      _ZN24CanvasRenderingContext2D8fillTextEPcdd(ctxPtr, strPtr, x, y) {
        const ctx = ptrToRef(ctxPtr);
        const str = ptrToString(strPtr);
        CanvasRenderingContext2D.prototype.fillText.apply(ctx, [str, x, y]);
      },
      _ZN24CanvasRenderingContext2D8set_fontEPc(ctxPtr, strPtr) {
        const ctx = ptrToRef(ctxPtr);
        const str = ptrToString(strPtr);
        ctx.font = str;
      },
      _ZN24CanvasRenderingContext2D9clearRectEdddd(ctxPtr, x, y, w, h) {
        const ctx = ptrToRef(ctxPtr);
        CanvasRenderingContext2D.prototype.clearRect.apply(ctx, [x, y, w, h]);
      },
      _ZN24CanvasRenderingContext2D4saveEv(ctxPtr) {
        const ctx = ptrToRef(ctxPtr);
        CanvasRenderingContext2D.prototype.save.apply(ctx, []);
      },
      _ZN24CanvasRenderingContext2D6lineToEdd(ctxPtr, x, y) {
        const ctx = ptrToRef(ctxPtr);
        CanvasRenderingContext2D.prototype.lineTo.apply(ctx, [x, y]);
      },
      _ZN24CanvasRenderingContext2D6moveToEdd(ctxPtr, x, y) {
        const ctx = ptrToRef(ctxPtr);
        CanvasRenderingContext2D.prototype.moveTo.apply(ctx, [x, y]);
      },
      _ZN24CanvasRenderingContext2D6strokeEv(ctxPtr) {
        const ctx = ptrToRef(ctxPtr);
        CanvasRenderingContext2D.prototype.stroke.apply(ctx, []);
      },
      _ZN24CanvasRenderingContext2D7restoreEv(ctxPtr) {
        const ctx = ptrToRef(ctxPtr);
        CanvasRenderingContext2D.prototype.restore.apply(ctx, []);
      },
      _ZN24CanvasRenderingContext2D9beginPathEv(ctxPtr) {
        const ctx = ptrToRef(ctxPtr);
        CanvasRenderingContext2D.prototype.beginPath.apply(ctx, []);
      },
      _ZN24CanvasRenderingContext2D9translateEdd(ctxPtr, x, y) {
        const ctx = ptrToRef(ctxPtr);
        CanvasRenderingContext2D.prototype.translate.apply(ctx, [x, y]);
      },
      _ZN9ImageData4new_Edd(retPtr, w, h) {
        const ret = new ImageData(w, h);
        i32[retPtr] = refToIdx(ret);
      },
      _ZN9ImageData10get_heightEv(imagePtr) {
        const image = ptrToRef(imagePtr);
        return image.height;
      },
      _ZN9ImageData8get_dataEv(retPtr, imagePtr) {
        const image = ptrToRef(imagePtr);
        i32[retPtr] = refToIdx(image.data);
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
