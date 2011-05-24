FIND_PACKAGE(BISON REQUIRED)
FIND_PACKAGE(FLEX REQUIRED)
BISON_TARGET(json_parser json_parser.y ${CMAKE_CURRENT_BINARY_DIR}/json_parser.c)
FLEX_TARGET(json_scanner json_scanner.l ${CMAKE_CURRENT_BINARY_DIR}/json_scanner.c)
ADD_FLEX_BISON_DEPENDENCY(json_scanner json_parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(RUOK_JSON_SOURCES
jsonmain.c
${BISON_json_parser_OUTPUTS}
${FLEX_json_scanner_OUTPUTS}
)

include_directories(${RUOK_JSON_SOURCE_DIR})
add_library(json STATIC ${RUOK_JSON_SOURCES})
SET_TARGET_PROPERTIES(json PROPERTIES OUTPUT_NAME json)
