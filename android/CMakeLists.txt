cmake_minimum_required(VERSION 3.4.1)

add_definitions(-DOLMLIB_VERSION_MAJOR=1)
add_definitions(-DOLMLIB_VERSION_MINOR=0)
add_definitions(-DOLMLIB_VERSION_PATCH=0)

set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "-DFOLLY_NO_CONFIG=1 -DFOLLY_HAVE_CLOCK_GETTIME=1 -DFOLLY_HAVE_MEMRCHR=1 -DFOLLY_USE_LIBCPP=1 -DFOLLY_MOBILE=1 -DON_ANDROID")

set (PACKAGE_NAME "cpp")
set (BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

add_library(
        ${PACKAGE_NAME}
        SHARED
        ../adapter/olm_account.cpp
        ../adapter/olm_session.cpp
        ../jsi_host_objects/olm_account_host.cpp
        ../jsi_host_objects/olm_session_host.cpp
        ../jsi-adapter/jsi_adapter.cpp
        ./cpp-adapter.cpp
        ../olm/src/account.cpp
        ../olm/src/base64.cpp
        ../olm/src/cipher.cpp
        ../olm/src/crypto.cpp
        ../olm/src/memory.cpp
        ../olm/src/message.cpp
        ../olm/src/pickle.cpp
        ../olm/src/ratchet.cpp
        ../olm/src/session.cpp
        ../olm/src/utility.cpp
        ../olm/src/pk.cpp
        ../olm/src/sas.c

        ../olm/src/ed25519.c
        ../olm/src/error.c
        ../olm/src/inbound_group_session.c
        ../olm/src/megolm.c
        ../olm/src/olm.cpp
        ../olm/src/outbound_group_session.c
        ../olm/src/pickle_encoding.c

        ../olm/lib/crypto-algorithms/aes.c
        ../olm/lib/crypto-algorithms/sha256.c
        ../olm/lib/curve25519-donna/curve25519-donna.c
        )

target_include_directories(
        ${PACKAGE_NAME}
        PRIVATE
        "${NODE_MODULES_DIR}/react-native/React"
        "${NODE_MODULES_DIR}/react-native/React/Base"
        "${NODE_MODULES_DIR}/react-native/ReactAndroid/src/main/jni"
        "${NODE_MODULES_DIR}/react-native/ReactAndroid/src/main/java/com/facebook/react/turbomodule/core/jni"
        "${NODE_MODULES_DIR}/react-native/ReactCommon"
        "${NODE_MODULES_DIR}/react-native/ReactCommon/callinvoker"
        "${NODE_MODULES_DIR}/react-native/ReactCommon/jsi"
        "../cpp"
        "../adapter"
        "../jsi_host_objects"
        "../jsi-adapter"
        "../olm"
        "../olm/include"
        "../olm/lib"
)

# find libraries

file (GLOB LIBRN_DIR "${BUILD_DIR}/react-native-0*/jni/${ANDROID_ABI}")


find_library(
        LOG_LIB
        log
)

find_library(
        REACT_NATIVE_JNI_LIB
        reactnativejni
        PATHS ${LIBRN_DIR}
        NO_CMAKE_FIND_ROOT_PATH
)

find_library(
        REACT_NATIVE_UTILS_LIB
        reactnativeutilsjni
        PATHS ${LIBRN_DIR}
        NO_CMAKE_FIND_ROOT_PATH
)

target_link_libraries(
        ${PACKAGE_NAME}
        ${LOG_LIB}
        ${REACT_NATIVE_JNI_LIB}
        ${REACT_NATIVE_UTILS_LIB}
        android
)
