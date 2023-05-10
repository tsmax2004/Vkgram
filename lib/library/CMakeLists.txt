function(project_log message)
    message(STATUS "[${PROJECT_NAME}] ${message}")
endfunction()

include(FetchContent)

# Libraries

# --------------------------------------------------------------------

# fmt with println

project_log("FetchContent: fmt")

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 87c066a35b7cc70bb7d438a543c8b49b577e61f4
)
FetchContent_MakeAvailable(fmt)

# --------------------------------------------------------------------

# pqxx for postgresql

project_log("FetchContent: pqxx")

FetchContent_Declare(
        pqxx
        GIT_REPOSITORY https://github.com/jtv/libpqxx
        GIT_TAG 78e4f95afe0cd4667d0accea83580ae279e0e9d3
)
FetchContent_MakeAvailable(pqxx)

# --------------------------------------------------------------------