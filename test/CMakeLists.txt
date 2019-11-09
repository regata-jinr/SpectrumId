set(test_SRC 
      Test.cxx)
      #      WindowTest.cxx 
      #UtilitiesTest.cxx )

add_executable(RunGoogleTests ${test_SRC})
target_link_libraries(RunGoogleTests ${GTEST_BOTH_LIBRARIES} lSpectrum ${ROOT_LIBRARIES})

add_test( NAME
          RunGoogleTests 
          COMMAND
          ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/test/RunGoogleTests
          )
