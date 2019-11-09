add_executable(spectrumId spectrumId.cpp)
target_link_libraries(spectrumId lSpectrum lRSpectrum ${ROOT_LIBRARIES} /usr/local/lib/libSpectrum.so)
