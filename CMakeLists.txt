# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (brokerZMQ)

#---- Enable C++14 ---- ////////////////
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET ( SOURCES
  brokerZMQ.cpp
)

SET (SOURCES_worker
  workerZMQ.cpp
)

SET (SOURCES_client
  clientZMQ.cpp
)

# Headers set
SET ( HEADERS
  zhelpers.hpp
)

SET( LIBS ${LIBS} -lzmq )

ADD_EXECUTABLE( brokerZMQ  ${SOURCES} )
TARGET_LINK_LIBRARIES( brokerZMQ ${LIBS} )

ADD_EXECUTABLE( workerZMQ  ${SOURCES_worker} )
TARGET_LINK_LIBRARIES( workerZMQ ${LIBS} )

ADD_EXECUTABLE( clientZMQ  ${SOURCES_client} )
TARGET_LINK_LIBRARIES( clientZMQ ${LIBS} )

