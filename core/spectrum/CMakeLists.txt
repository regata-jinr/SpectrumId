set(SPECTRUM_SOURCES
  inc/Spectrum.h
  inc/MetaData.h
  src/Spectrum.cxx
)

add_library(lSpectrum STATIC SPECTRUM_SOURCES)

target_link_libraries(lSpectrum lPeak lBackground lWindow)

target_include_directories(lSpectrum PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

