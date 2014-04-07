include_directories(${GTEST_INCLUDE_DIR})

# .cxx sources
set(ldb_SRCS_CXX
	database.cpp
	layer.cpp
	layer_transaction.cpp
	layer_iterator.cpp
	memory_iterator.cpp
	database_iterator.cpp
)

# .h headers
set(ldb_SRCS_HEADERS
	defs.h
	database_abstract.h
	database.h
	layer.h
	iterator_abstract.h
	layer_transaction.h
	layer_iterator.h
	memory_iterator.h
	database_iterator.h
)

add_library(ldblayer SHARED ${ldb_SRCS_CXX} ${ldb_SRCS_HEADERS})