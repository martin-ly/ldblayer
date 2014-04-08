include_directories(${GTEST_INCLUDE_DIR})

# .cxx sources
set(ldb_SRCS_CXX
	database.cpp
	database_transaction.cpp
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
	transaction_abstract.h
	iterator_abstract.h
	database.h
	database_transaction.h
	database_iterator.h
	layer.h	
	layer_transaction.h
	layer_iterator.h
	memory_iterator.h
)

add_library(ldblayer SHARED ${ldb_SRCS_CXX} ${ldb_SRCS_HEADERS})