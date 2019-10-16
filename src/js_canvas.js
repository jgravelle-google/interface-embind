let lastTime = Date.now();
let t = 0.0;

function rects(ctx) {
  ctx.fillStyle = 'rgb(200, 0, 0)'
  ctx.fillRect(10, 10, 200, 200);
  ctx.fillStyle = '#11cc22'
  ctx.fillRect(40, 40, 160, 160);
  ctx.fillStyle = 'blue'
  ctx.fillRect(70, 50, 120, 120);
}

function sines(ctx) {
  const x0 = 50;
  const y0 = 400;
  const w = 700;
  const h = 300;
  const segments = 256;
  const lines = 7;
  const freq = 2;
  ctx.save();
  ctx.translate(x0, y0);
  const tau = 2 * Math.PI;
  for (let j = 1; j <= lines; ++j) {
    const off = j / lines * t * tau / 3;
    const f = freq + 0.3 * Math.cos(off / 5.1);
    ctx.beginPath();
    ctx.moveTo(0, h / 2 * Math.sin(off));
    for (let i = 1; i < segments; ++i) {
      const x = i * w / segments;
      const y = h / 2 * Math.sin(i * f / segments * tau + off);
      ctx.lineTo(x, y);
    }
    ctx.stroke();
  }
  ctx.restore();
}

function image(ctx) {
  const img = new ImageData(500, 200);
  for (let i = 0; i < img.data.length; i += 4) {
    const p = i / 4;
    const x = p % img.width;
    const y = p / img.width;
    const u = x / img.width;
    const v = y / img.height;
    img.data[i + 0] = 255 * Math.abs(Math.sin(t)) * u;
    img.data[i + 1] = 255 * Math.abs(Math.cos(t)) * v;
    img.data[i + 2] = 255 * (1.0 - u * v);
    img.data[i + 3] = 255;
  }
  ctx.putImageData(img, 260, 10);
}

function framerate(ctx, dT) {
  ctx.font = '20px sans';
  ctx.fillStyle = '#444444';
  const ms = (1000 * dT) | 0;
  ctx.fillText('Frame time: ' + ms + 'ms', 600, 585)
}

function frame() {
  let curTime = Date.now();
  let dT = (curTime - lastTime) / 1000;
  lastTime = curTime;
  t += dT;

  const canvas = document.getElementById('canvas');
  const ctx = canvas.getContext('2d');
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  rects(ctx);
  sines(ctx);
  image(ctx);
  framerate(ctx, dT);

  requestAnimationFrame(frame);
}

requestAnimationFrame(frame);
