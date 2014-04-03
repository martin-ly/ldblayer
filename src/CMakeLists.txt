include_directories(${GTEST_INCLUDE_DIR})

# .cxx sources
set(ldb_SRCS_CXX
    database.cpp
    layer.cpp
    layer_transaction.cpp
    layer_iterator.cpp
)

# .h headers
set(ldb_SRCS_HEADERS
    database_abstract.h
    database.h
    layer.h
    layer_transaction.h
    layer_iterator.h
)

add_library(ldb SHARED ${ldb_SRCS_CXX} ${ldb_SRCS_HEADERS})