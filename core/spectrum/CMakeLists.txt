set(spectrum_SRC 
      src/Background.cxx
      src/Peak.cxx
      src/Spectrum.cxx
      src/Window.cxx )
   
add_library(lSpectrum SHARED ${spectrum_SRC})
