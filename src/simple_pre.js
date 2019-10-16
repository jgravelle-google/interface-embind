var Module = {
  onRuntimeInitialized() {
    for (let i = 0; i < Module.myConst; ++i) {
      console.log('fact(' + i + '): ' + Module.fact(i));
    }
  },
};
