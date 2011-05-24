set(RUOK_SOURCES
main.cc
options.cc
options.h
requestthread.cc
requestthread.h
responseprinter.cc
responseprinter.h
)
include_directories(${RUOK_SOURCE_DIR} ${RUOK_CURL_INCLUDES} ${RUOK_LIBXML2_INCLUDES})
add_executable(ruok ${RUOK_SOURCES})
target_link_libraries(ruok ${RUOK_LIBXML2_LIBRARIES} ${RUOK_CURL_LIBRARIES} json)

INSTALL(PROGRAMS ruok ${CMAKE_CURRENT_BINARY_DIR}/ruok 
	DESTINATION bin)
