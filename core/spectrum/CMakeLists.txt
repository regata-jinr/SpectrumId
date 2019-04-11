set(spectrum_SRC 
      src/Background.cxx
      src/Peak.cxx
      src/Spectrum.cxx
      src/Window.cxx 
      src/Utilities.cxx)
   
add_library(lSpectrum SHARED ${spectrum_SRC})
