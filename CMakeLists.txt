cmake_minimum_required(VERSION 3.1)
project(nunchuk VERSION 0.1.0)

configure_file(src/config.h.in nunchuk-config.h)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)
set(Boost_USE_STATIC_LIBS ON)
set(OPENSSL_USE_STATIC_LIBS ON)
set(UR__DISABLE_TESTS ON)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

find_package(Threads REQUIRED)
find_package(Boost 1.47.0 REQUIRED COMPONENTS filesystem program_options thread)
find_package(OpenSSL REQUIRED)

if(WIN32)
    find_package(sqlcipher CONFIG REQUIRED)
else()
    find_library(sqlcipher_lib NAMES libsqlcipher.a PATHS "${PROJECT_SOURCE_DIR}/contrib/sqlcipher/.libs" NO_DEFAULT_PATH)
    if (NOT sqlcipher_lib)
        message(FATAL_ERROR "sqlcipher not found")
    endif()
    add_library(sqlcipher STATIC IMPORTED)
    set_target_properties(sqlcipher PROPERTIES IMPORTED_LOCATION ${sqlcipher_lib})
endif()

add_subdirectory(embedded)
add_subdirectory(contrib/bc-ur-cpp)

add_library(trezor-crypto STATIC
    "contrib/trezor-firmware/crypto/bip39.c"
    "contrib/trezor-firmware/crypto/hmac.c"
    "contrib/trezor-firmware/crypto/memzero.c"
    "contrib/trezor-firmware/crypto/pbkdf2.c"
    "contrib/trezor-firmware/crypto/sha2.c"
    "contrib/trezor-firmware/crypto/aes/aes_modes.c"
    "contrib/trezor-firmware/crypto/aes/aescrypt.c"
    "contrib/trezor-firmware/crypto/aes/aeskey.c"
    "contrib/trezor-firmware/crypto/aes/aestab.c"
)

target_include_directories(trezor-crypto PUBLIC 
    "${PROJECT_SOURCE_DIR}/contrib/trezor-firmware/crypto"
)

if(WIN32)
    include(TestBigEndian)
    test_big_endian(BYTE_ORDER_BIG_ENDIAN)
    add_compile_definitions(LITTLE_ENDIAN=1234)
    add_compile_definitions(BIG_ENDIAN=4321)
    add_compile_definitions(__ORDER_LITTLE_ENDIAN__=LITTLE_ENDIAN)
    add_compile_definitions(__ORDER_BIG_ENDIAN__=BIG_ENDIAN)
    if (BYTE_ORDER_BIG_ENDIAN)
        add_compile_definitions(BYTE_ORDER=BIG_ENDIAN)
        add_compile_definitions(__BYTE_ORDER__=BIG_ENDIAN)
    else()
        add_compile_definitions(BYTE_ORDER=LITTLE_ENDIAN)
        add_compile_definitions(__BYTE_ORDER__=LITTLE_ENDIAN)
    endif()
endif()

add_library(ur-2 STATIC 
    "contrib/bc-ur-2/src/crc32.c"
    "contrib/bc-ur-2/src/memzero.c"
    "contrib/bc-ur-2/src/sha2.c"
    "contrib/bc-ur-2/src/bytewords.cpp"
    "contrib/bc-ur-2/src/fountain-decoder.cpp"
    "contrib/bc-ur-2/src/fountain-encoder.cpp"
    "contrib/bc-ur-2/src/fountain-utils.cpp"
    "contrib/bc-ur-2/src/random-sampler.cpp"
    "contrib/bc-ur-2/src/ur-decoder.cpp"
    "contrib/bc-ur-2/src/ur-encoder.cpp"
    "contrib/bc-ur-2/src/ur.cpp"
    "contrib/bc-ur-2/src/utils.cpp"
    "contrib/bc-ur-2/src/xoshiro256.cpp"
)

target_include_directories(ur-2 PUBLIC 
    "${PROJECT_SOURCE_DIR}/contrib/bc-ur-2/src"
    ${Boost_INCLUDE_DIRS}
)

add_library("${PROJECT_NAME}" STATIC 
    src/hwiservice.cpp 
    src/coreutils.cpp
    src/descriptor.cpp
    src/signingprovider.cpp
    src/coinselector.cpp
    src/softwaresigner.cpp
    src/nunchukimpl.cpp
    src/nunchukutils.cpp
    src/nunchukmatriximpl.cpp
    src/backend/synchronizer.cpp
    src/backend/electrum/client.cpp
    src/backend/electrum/synchronizer.cpp
    src/backend/corerpc/client.cpp
    src/backend/corerpc/synchronizer.cpp
    src/dto/appsettings.cpp
    src/dto/device.cpp
    src/dto/mastersigner.cpp
    src/dto/singlesigner.cpp
    src/dto/transaction.cpp
    src/dto/unspentoutput.cpp
    src/dto/wallet.cpp
    src/dto/roomevent.cpp
    src/dto/roomtransaction.cpp
    src/dto/roomwallet.cpp
    src/storage/appstatedb.cpp
    src/storage/common.cpp
    src/storage/db.cpp
    src/storage/signerdb.cpp
    src/storage/storage.cpp
    src/storage/walletdb.cpp
    src/storage/roomdb.cpp
    src/utils/loguru.cpp
)

set(PROJECT_LIBRARIES
    embedded
    ur
    ur-2
    OpenSSL::SSL OpenSSL::Crypto
    Threads::Threads
)
                            
if(WIN32)
    target_compile_options("${PROJECT_NAME}" PUBLIC /MT)
    target_link_options("${PROJECT_NAME}" PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
    set(PROJECT_LIBRARIES sqlcipher::sqlcipher ${PROJECT_LIBRARIES} crypt32 ws2_32)
elseif(APPLE)
    set(PROJECT_LIBRARIES sqlcipher ${PROJECT_LIBRARIES})
elseif(UNIX)
    set(PROJECT_LIBRARIES sqlcipher ${PROJECT_LIBRARIES} dl)
endif()

target_link_libraries("${PROJECT_NAME}" PUBLIC ${PROJECT_LIBRARIES} trezor-crypto)
target_include_directories("${PROJECT_NAME}" PUBLIC 
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/include"
    ${Boost_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
    "${PROJECT_SOURCE_DIR}/contrib"
    "${PROJECT_BINARY_DIR}"
)

include(CTest)
add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
