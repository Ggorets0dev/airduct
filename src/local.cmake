# Add source files
file(GLOB_RECURSE SRC_CC "src/*.cpp" SRC_H "src/*.hpp")

configure_file(
	"${PROJECT_SOURCE_DIR}/include/ver.h.in"
	"${PROJECT_BINARY_DIR}/version.h"
	)

add_executable(airduct ${SRC_CC} ${SRC_H})

add_dependencies(airduct rapidjson)
