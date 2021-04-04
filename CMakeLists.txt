cmake_minimum_required(VERSION 3.0.0)
project(game_project CXX C)

# ENGINE
find_package(SDL2 REQUIRED)

include_directories(src)
include_directories(include)

file(GLOB_RECURSE SRCS src/*.cpp src/*.c include/*.hpp include/*.h)
add_library(engine SHARED ${SRCS})

# target_include_directories(engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(engine PUBLIC cxx_std_20)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/imgui_src)
target_link_libraries(engine PRIVATE imgui)

target_link_libraries(engine PRIVATE SDL2::SDL2 SDL2::SDL2main )
target_link_libraries(engine PRIVATE GL)
# target_compile_options(engine PRIVATE
#                         $<$<CXX_COMPILER_ID:MSVC>:/W4>
#                         $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -pedantic -fsanitize=leak -pg>
#                         $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -pedantic>
#                         )
# ENGINE


# GAME
file(GLOB_RECURSE GAME_SRCS game_src/*.cpp game_src/*.hpp)
add_library(game SHARED ${GAME_SRCS})
target_compile_features(game PUBLIC cxx_std_20)
target_link_libraries(game PRIVATE engine )

target_compile_options(game PRIVATE
                        $<$<CXX_COMPILER_ID:MSVC>:/W4>
                        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -pedantic -fsanitize=leak -pg>
                        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -pedantic>
                        )
# GAME

add_executable(start main.cpp)
target_compile_features(start PUBLIC cxx_std_20)
target_link_libraries(start PRIVATE engine game)

file(GLOB_RECURSE RES res/*.*)
file(COPY ${RES} DESTINATION ./res/)

file(GLOB_RECURSE FONTS fonts/*.*)
file(COPY ${FONTS} DESTINATION ./fonts/)

file(GLOB_RECURSE SHADER shader/*.*)
file(COPY ${SHADER} DESTINATION ./shader/)



# Install
install(TARGETS start
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/lib
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/lib)
# install(FILES  resource/Star.ppm  DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)

