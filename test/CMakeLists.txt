add_executable(to_json_test to_json_test.cpp)
target_link_libraries(to_json_test PUBLIC kie_json)
add_test(to_json_test to_json_test)

add_executable(from_json_test from_json_test.cpp)
target_link_libraries(from_json_test PUBLIC kie_json)
add_test(from_json_test from_json_test)

add_executable(field_test field_test.cpp)
target_link_libraries(field_test PUBLIC kie_json)
add_test(field_test field_test)