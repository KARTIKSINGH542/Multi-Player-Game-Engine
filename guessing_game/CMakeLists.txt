cmake_minimum_required(VERSION 3.10)
project(NumberGuessingGame CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(gamecore
    src/Participant.cpp src/Rules.cpp src/Scoreboard.cpp src/Umpire.cpp src/Game.cpp)
target_include_directories(gamecore PUBLIC include)
if(MSVC)
  target_compile_options(gamecore PRIVATE /W4)
else()
  target_compile_options(gamecore PRIVATE -Wall -Wextra -pedantic)
endif()

add_executable(guessing_game src/main.cpp)
target_link_libraries(guessing_game gamecore)

enable_testing()
add_executable(test_game tests/test_game.cpp)
target_link_libraries(test_game gamecore)
add_test(NAME test_game COMMAND test_game)
