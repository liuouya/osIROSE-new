CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
PROJECT("osIROSE")

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

INCLUDE(OsIROSECMakeTools)

SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 1)
SET(VERSION_PATCH 0)
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# Evaluate ABI version number.
MATH(EXPR PROJECT_ABI_VERSION_SUFFIX "${VERSION_MAJOR} * 10000 + ${VERSION_MINOR} * 100 + ${VERSION_PATCH} * 1")

SET(PROJECT_ABI_VERSION v${PROJECT_ABI_VERSION_SUFFIX})

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

OPTION(PROJECT_DEBUG 
  "Enable debugging messages support" ON)
OPTION(ENABLE_TESTING 
  "Enable testing" OFF)
OPTION(ENABLE_TESTING_COVERAGE, 
  "Enable generating code coverage report" OFF)
OPTION(BUILD_TOOLS 
  "Build server tools" ON)
OPTION(BUILD_PROTOBUF 
  "Build Protobuf (Leave this off)" OFF)
OPTION(BUILD_API_DOC
  "Build doxygen documentation" OFF)
OPTION(ENABLE_MYSQL_ONE_INSTANCE 
	"Enable use of the single instance mysql database" ON)
  
IF (BUILD_API_DOC)
  FIND_PACKAGE(Doxygen)
  IF (DOXYGEN_FOUND)
    ADD_SUBDIRECTORY(doc)
  ENDIF (DOXYGEN_FOUND)
ENDIF (BUILD_API_DOC)

IF(ENABLE_MYSQL_ONE_INSTANCE)
  SET(ENABLE_MYSQL ON)
ENDIF()

IF(ENABLE_MYSQL)
  find_package(MySQL REQUIRED)
ENDIF()

foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
   if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
   endif(${flag_var} MATCHES "/MD")
endforeach(flag_var)

IF(ENABLE_TESTING)
	SET(CMAKE_RULE_MESSAGES OFF)	
	IF (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
		#Build GoogleMock
		ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/tools/googletest/googlemock)		
	ELSE()
		#Build GoogleMock
		ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/tools/googletest/googlemock)
		SET(GMOCK_INCLUDE_DIR ${gmock_SOURCE_DIR}/include)
		SET(GMOCK_BINARY_DIR ${gmock_SOURCE_DIR})
		SET(GTEST_INCLUDE_DIR ${gtest_SOURCE_DIR}/include)
		SET(GTEST_BINARY_DIR ${gtest_SOURCE_DIR})
	ENDIF()
	SET(CMAKE_RULE_MESSAGES ON)
ENDIF()

SET(ASIO_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/tools/asio/asio/include)
SET(EXTERNAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdParty/include)
SET(EXTERNAL_BINARY_DIR ${CMAKE_SOURCE_DIR}/3rdParty/lib)

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	#------------
	#WINDOWS ONLY SETTINGS!!!!
	#------------
	#Build Protobuf
	IF(ENABLE_MYSQL)
	 ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/mysqlpp)
	 SET(MYSQL_LIBRARIES libmysql.lib CACHE STRING "")
	 SET(MYSQLpp_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/mysqlpp)
	ENDIF()
	SET(PROTOBUF_ROOT ${CMAKE_SOURCE_DIR}/3rdParty CACHE STRING "")
	SET(PROTOBUF_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/tools/protobuf/src CACHE STRING "")
  SET(GMOCK_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/tools/googletest/googlemock/include)
  SET(GMOCK_BINARY_DIR ${gmock_SOURCE_DIR})
  SET(GTEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/tools/googletest/googletest/include)
	SET(GTEST_BINARY_DIR ${gtest_SOURCE_DIR})
ELSE()
	#UNIX ONLY SETTINGS

	#Set Protobuf Paths
	SET(PROTOBUF_ROOT ${CMAKE_SOURCE_DIR}/protobuf)
	SET(PROTOBUF_BINARY_DIR ${PROTOBUF_ROOT}/lib)
	IF(ENABLE_MYSQL)
		SET(MYSQL_INCLUDE_DIR /usr/include/mysql)
		SET(MYSQLpp_INCLUDE_DIR /usr/include/mysql++)
		SET(MYSQL_BINARY_DIR /usr/lib)
	ENDIF()
	SET(LINK_FLAGS "${LINK_FLAGS} -lnsl -lz -lm")
ENDIF()

find_package(ProtocolBuffer REQUIRED)

ADD_SUBDIRECTORY(src)

IF(BUILD_TOOLS)
	ADD_SUBDIRECTORY(tools)
ENDIF()

