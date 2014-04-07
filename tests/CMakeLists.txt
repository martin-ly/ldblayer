include_directories("${CMAKE_SOURCE_DIR}/src")

message("${CMAKE_SOURCE_DIR}/src")

# .cxx sources
set(ldb_tests_SRCS_CXX
    direct_db.cpp
    direct_raw.cpp
    direct_transaction.cpp
    direct_iterator.cpp
    layer_db.cpp
    layer_raw.cpp
    layer_transaction.cpp
    layer_iterator.cpp
    memory_iterator.cpp
    db_iterator.cpp
)

# .h headers
set(ldb_tests_SRCS_HEADERS
    direct_db.h
    layer_db.h
    db_iterator.h
)


add_executable(ldb_tests ${ldb_tests_SRCS_CXX} ${ldb_tests_SRCS_HEADERS})
target_link_libraries(ldb_tests ldblayer ${LDB_LIBRARY} ${GTEST_BOTH_LIBRARIES})

GTEST_ADD_TESTS(ldb_tests "" ${ldb_tests_SRCS_CXX})