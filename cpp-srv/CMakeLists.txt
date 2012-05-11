cmake_minimum_required (VERSION 2.6)
set(Boost_DEBUG 1)
project (yafcd)
find_package(Boost REQUIRED system regex)
find_package(Threads)
link_directories(${Boost_LIBRARY_DIRS})

# add_library(chatsession chatsession.cpp)
# add_library(chatuser chatuser.cpp)
# add_library(server server.cpp)
file (GLOB SRC_FILES *.cpp *.h)
add_executable(yafcd ${SRC_FILES})
target_link_libraries(yafcd     
  ${Boost_LIBRARIES} 
  ${CMAKE_THREAD_LIBS_INIT})                    
set(CMAKE_CXX_FLAGS "-std=c++0x -g")

