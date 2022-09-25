set(A 1)
set(WORK_SPACE_PATH ${PROJECT_SOURCE_DIR})
set(TEST_STRING  "AAAAAAAAAAAAAAAAAAAA")
configure_file (
  ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/global_defination/global_defination.h.in
  ${PROJECT_BINARY_DIR}/include/${PROJECT_NAME}/global_defination/global_defination.h)
include_directories(${PROJECT_BINARY_DIR}/include)