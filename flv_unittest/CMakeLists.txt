#unittest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
project (flv_unittest)

set(TEST_SRCS
  parsinghflv_test.cpp
  vector_test.cpp
  flv_message_test.cpp
)

################################################################################

ADD_DEFINITIONS(-Wall -g -fno-strict-aliasing -D_LARGEFILE64_SOURCE -D_BARBADOS_=1 -D_REENTRANT -D_FILE_OFFSET_BITS=64)
add_compile_options(-DBOOST_COROUTINES_NO_DEPRECATION_WARNING=1)
add_executable(${PROJECT_NAME} ${TEST_SRCS})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "--coverage -std=c++11")
target_link_libraries(${PROJECT_NAME}
  MPEG2AVMux
  CiMediaParser  
  CiLocalFileIO
  CiMediaFile
  CiMPEG2Lib
  MPEG4Index
  MPEG2Utils
  MPEG4Utils
  CiTransErrors  
  CiUtils
  gtest_main
	gtest
	gcov
	dl
	rt
	uuid
	pthread
  ${Boost_LIBRARIES}
)
