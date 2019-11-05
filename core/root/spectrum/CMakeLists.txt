set(RSpectrum_SRC 
		  src/TSpectrum.cxx
		  src/TSpectrumFit.cxx
		  src/TSpectrumTransform.cxx
)
   
add_library(lRSpectrum SHARED ${RSpectrum_SRC})
