project(parser)

set(parser-source-files
    "${PROJECT_SOURCE_DIR}/objects/grammar.cpp"
    "${PROJECT_SOURCE_DIR}/objects/state.cpp"
    "${PROJECT_SOURCE_DIR}/objects/automata.cpp"
    "${PROJECT_SOURCE_DIR}/objects/table.cpp"
    "${PROJECT_SOURCE_DIR}/objects/reader.cpp"
    "${PROJECT_SOURCE_DIR}/objects/parser.cpp"
    "${PROJECT_SOURCE_DIR}/main.cpp"
    CACHE INTERNAL "")

set(parser-include-files
    "${Parser_SOURCE_DIR}/include/SOURCE.hpp"
    "${Parser_SOURCE_DIR}/include/objects/grammar.hpp"
    "${Parser_SOURCE_DIR}/include/objects/state.hpp"
    "${Parser_SOURCE_DIR}/include/objects/automata.hpp"
    "${Parser_SOURCE_DIR}/include/objects/table.hpp"
    "${Parser_SOURCE_DIR}/include/objects/reader.hpp"
    "${Parser_SOURCE_DIR}/include/objects/parser.hpp"
    CACHE INTERNAL "")

add_library(
    parser SHARED ${parser-source-files} ${parser-include-files}
)

add_executable(run_main "${PROJECT_SOURCE_DIR}/main.cpp")
target_link_libraries(run_main parser)
