if (NOT DRIO_VER)
    set (DRIO_VER "11.2.0")
endif ()

# Set up DynamoRIO download targets
if (BUILD_DRIO)
  FetchContent_Declare(DynamoRIO
    GIT_REPOSITORY  "https://github.com/DynamoRIO/dynamorio.git"
    GIT_TAG         "release_${DRIO_VERSION}"
    GIT_SHALLOW     TRUE
  )
elseif (DRIO_URL)
  FetchContent_Declare(DynamoRIO
      URL "${DRIO_URL}"
  )
else ()
  if (NOT DRIO_URL)
    if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        set (DRIO_URL "https://github.com/DynamoRIO/dynamorio/releases/download/release_${DRIO_VER}/DynamoRIO-Windows-${DRIO_VER}.zip")
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        set (DRIO_URL "https://github.com/DynamoRIO/dynamorio/releases/download/release_${DRIO_VER}/DynamoRIO-Linux-${DRIO_VER}.tar.gz")
    else ()
      message (FATAL_ERROR "Unsupported system: '${CMAKE_SYSTEM_NAME}'")
    endif ()
  endif()

  FetchContent_Declare(DynamoRIO
      URL "${DRIO_URL}"
  )
endif()

# Download and activate DynamoRIO
FetchContent_MakeAvailable(DynamoRIO)
set (DynamoRIO_DIR "${dynamorio_SOURCE_DIR}/cmake")
find_package(DynamoRIO)