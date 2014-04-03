include_directories(${GTEST_INCLUDE_DIR})

# .cxx sources
set(ldb_SRCS_CXX
    database.cpp
    layer.cpp
    LevelDatabaseTransaction.cpp
    LevelDatabaseIterator.cpp
)

# .h headers
set(ldb_SRCS_HEADERS
    database_abstract.h
    database.h
    layer.h
    LevelDatabaseTransaction.h
    LevelDatabaseIterator.h
)

add_library(ldb SHARED ${ldb_SRCS_CXX} ${ldb_SRCS_HEADERS})