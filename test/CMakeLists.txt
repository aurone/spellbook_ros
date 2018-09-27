find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_executable(grid_test grid_test.cpp)
target_link_libraries(grid_test spellbook)
target_include_directories(grid_test SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(grid_test PRIVATE ${Boost_LIBRARIES})

add_executable(spellbook_tests main.cpp)
target_link_libraries(spellbook_tests spellbook)

add_executable(matrix_test matrix_test.cpp)
target_include_directories(matrix_test SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(matrix_test PRIVATE spellbook)
target_link_libraries(matrix_test PRIVATE ${Boost_LIBRARIES})

#add_executable(line_test line_test.cpp)
#target_link_libraries(line_test ${Boost_LIBRARIES})

add_executable(rotations_test rotations_test.cpp)
target_include_directories(rotations_test SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(rotations_test PRIVATE spellbook)
target_link_libraries(rotations_test PRIVATE ${Boost_LIBRARIES})
