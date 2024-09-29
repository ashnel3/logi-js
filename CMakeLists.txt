cmake_minimum_required(VERSION 3.11)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)
project(logisdk LANGUAGES CXX)

# cmake modules
include(FetchContent)

# standards
add_definitions(-DNAPI_VERSION=4)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# global includes
include_directories(${CMAKE_SOURCE_DIR}/node_modules/node-addon-api)
include_directories(${CMAKE_JS_INC})

if(NOT MSVC)
    message(FATAL_ERROR "Error: Unsupported compiler, 'MSVC' is required!")
endif()

# === Fetch-Content ===

FetchContent_Declare(
    LEDSDK
    DOWNLOAD_EXTRACT_TIMESTAMP true
    URL https://www.logitechg.com/sdk/LED_SDK_9.00.zip
    URL_HASH SHA512=43dd9cf6186d84948cdd565399e0c748eb327a9b2d1c77ca0eaeb8cee013f5f71bc20bc97002917ce0d59cc96a3887f392cd7376cde60c692a371016ccf3215e
)
FetchContent_Declare(
    LCDSDK
    DOWNLOAD_EXTRACT_TIMESTAMP true
    URL https://www.logitechg.com/sdk/LCDSDK_8.57.148.zip
    URL_HASH SHA512=e7b1f22a968149ca39d743a536f9e88814b6cbe2110514aa120199ccfbfbcb2a4f8d7044e9916b1a777d2347d64791286591eb94194e926d92cec7f096c8f6cd
)
FetchContent_Declare(
    GKEYSDK
    DOWNLOAD_EXTRACT_TIMESTAMP true
    URL https://www.logitechg.com/sdk/GkeySDK_8.57.148.zip
    URL_HASH SHA512=20c55d5311b9a4ba96a40306afddfc99587dcc05ebef1642cf85676f2827703668cd234a16c0aa9f8ee5217965dcce9cf634514b237eddaab48fd9a0e5ac8963
)
FetchContent_MakeAvailable(GKEYSDK LCDSDK LEDSDK)

# === Targets ===

# GKeySdk
FetchContent_GetProperties(GKEYSDK SOURCE_DIR GKEYSDK_SOURCE)
add_library(libgkey SHARED src/libgkey.cc ${CMAKE_JS_SRC})
set_target_properties(libgkey PROPERTIES PREFIX "" SUFFIX ".node")
target_include_directories(libgkey PRIVATE ${GKEYSDK_SOURCE}/Include)
target_link_libraries(libgkey PRIVATE
    ${CMAKE_JS_LIB}
    ${GKEYSDK_SOURCE}/Lib/x64/LogitechGkeyLib.lib
)

# LedSdk
FetchContent_GetProperties(LEDSDK SOURCE_DIR LEDSDK_SOURCE)
add_library(libled SHARED src/libled.cc ${CMAKE_JS_SRC})
set_target_properties(libled PROPERTIES PREFIX "" SUFFIX ".node")
target_include_directories(libled PRIVATE ${LEDSDK_SOURCE}/Include)
target_link_libraries(libled PRIVATE
    ${CMAKE_JS_LIB}
    ${LEDSDK_SOURCE}/Lib/x64/LogitechLEDLib.lib
)

# LcdSdk
FetchContent_GetProperties(LCDSDK SOURCE_DIR LCDSDK_SOURCE)
add_library(liblcd SHARED src/liblcd.cc ${CMAKE_JS_SRC})
set_target_properties(liblcd PROPERTIES PREFIX "" SUFFIX ".node")
target_include_directories(liblcd PRIVATE ${LCDSDK_SOURCE}/Include)
target_link_libraries(liblcd PRIVATE
    ${CMAKE_JS_LIB}
    ${LCDSDK_SOURCE}/Lib/x64/LogitechLCDLib.lib
)

# === Before Close ===

# generate libs
if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()
