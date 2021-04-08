file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(imgui STATIC ${sources} ${headers})
target_compile_features(imgui PUBLIC cxx_std_20)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET imgui PROPERTY POSITION_INDEPENDENT_CODE ON)
