project(test_init)

set(TEST_INIT_SOURCE test_init.cpp)

add_executable(${PROJECT_NAME} ${TEST_INIT_SOURCE})

target_link_libraries(${PROJECT_NAME} cdr cutest)

add_test(test_init ./test_init)