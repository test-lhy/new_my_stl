find_package(OpenSSL)
if (OPENSSL_FOUND)
    message(STATUS "OPENSSL_VERSION = ${OPENSSL_VERSION}")
    message(STATUS "OPENSSL_SSL_LIBRARY = ${OPENSSL_SSL_LIBRARY}")
    message(STATUS "OPENSSL_CRYPTO_LIBRARY = ${OPENSSL_CRYPTO_LIBRARY}")
endif ()

set(CONCPP_LIB_DIR "/usr/local/mysql/connector-c++-8.2.0/lib64/debug")
set(CONCPP_INCLUDE_DIR "/usr/local/mysql/connector-c++-8.2.0/include")

set(find_name mysqlcppconn8-static)
set(find_dir "${CONCPP_LIB_DIR}")

find_library(CONCPP_LIB
        NAMES ${find_name}
        PATHS "${find_dir}"
        NO_DEFAULT_PATH
)

find_library(CONCPP_LIB_DEBUG
        NAMES ${find_name}
        PATHS "${find_dir}"
        NO_DEFAULT_PATH
)

if (NOT CONCPP_LIB AND NOT CONCPP_LIB_DEBUG)
    message(FATAL_ERROR
            "Could not find Connector/C++ libraries at: ${find_dir}"
    )
endif ()

message(STATUS "Using connector lib at: ${CONCPP_LIB}")

# Define imported target for the connector library.

add_library(MySQL::conncpp STATIC IMPORTED)

set_target_properties(MySQL::conncpp PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CONCPP_INCLUDE_DIR}"
        IMPORTED_LOCATION "${CONCPP_LIB}"
        IMPORTED_LOCATION_DEBUG "${CONCPP_LIB_DEBUG}"
)

# We need to link with libraries on which connector depends
# (however, no need to do it on Windows)

if (NOT WIN32)
    # Connector/C++ requires these libraries on Unix
    if (NOT APPLE)
        set_property(TARGET MySQL::conncpp
                APPEND PROPERTY INTERFACE_LINK_LIBRARIES pthread # rt
        )
    endif ()

    # If connector uses OpenSSL libraries, we need to link to them too.
    if (OPENSSL_FOUND)
        set_property(TARGET MySQL::conncpp
                APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY}
        )
    endif ()

    if (NOT CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
        # Connector needs to Link with libresolv
        set_property(TARGET MySQL::conncpp
                APPEND PROPERTY INTERFACE_LINK_LIBRARIES resolv
        )
    endif ()
endif ()

# To use DevAPI we must enable C++11
# Use clang's native C++ runtime library to match the one used
# by Connector/C++.
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif ()

# When linking with static library, this macro must be defined.
if (BUILD_STATIC)
    add_definitions(-DSTATIC_CONCPP)
endif ()