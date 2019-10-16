var Module = {
  onRuntimeInitialized() {
    Module.init();
    function frame() {
      Module.frame();
      requestAnimationFrame(frame);
    }
    frame();
  },
};
