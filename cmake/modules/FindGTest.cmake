if (NOT GTEST_VER)
    set (GTEST_VER "1.17.0")
endif ()

# Set up GTest download targets
if (BUILD_TEST)
  FetchContent_Declare(GTest
    GIT_REPOSITORY  "https://github.com/google/googletest.git"
    GIT_TAG         "v${GTEST_VER}"
    GIT_SHALLOW     TRUE
  )
elseif (GTEST_URL)
  FetchContent_Declare(DynaGTestmoRIO
      URL "${GTEST_URL}"
  )
else ()
  if (NOT GTEST_URL)
    if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        set (GTEST_URL "https://github.com/google/googletest/archive/refs/tags/v{GTEST_VER}.zip")
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        set (GTEST_URL "https://github.com/google/googletest/archive/refs/tags/v{GTEST_VER}.tar.gz")
    else ()
      message (FATAL_ERROR "Unsupported system: '${CMAKE_SYSTEM_NAME}'")
    endif ()
  endif()

  FetchContent_Declare(GTest
      URL "${GTEST_URL}"
  )
endif()

# Download and activate GTest
FetchContent_MakeAvailable(GTest)
set (GTest_DIR "${gtest_SOURCE_DIR}/cmake")