cmake_minimum_required (VERSION 3.20)

project (itm_linux
         VERSION 1.0.0
         LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(CMakePackageConfigHelpers)
include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ABI 64 CACHE STRING "The ABI of the ITM Longley-Rice libraries and executables.")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ABI 32 CACHE STRING "The ABI of the ITM Longley-Rice libraries and executables.")
endif ()
message("ABI                : ${ABI}")

string(APPEND CMAKE_ADDTIONAL_CXX_FLAGS " -m${ABI}")
string(APPEND CMAKE_SHARED_LINKER_FLAGS " -m${ABI}")
string(APPEND CMAKE_EXE_LINKER_FLAGS " -m${ABI}")

# Enable testing for all systems
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_ADDTIONAL_CXX_FLAGS} -Wall -Wextra -Wno-deprecated-declarations -Wold-style-cast")

add_library(ITModel SHARED
  src/ComputeDeltaH.cpp
  src/DiffractionLoss.cpp
  src/FindHorizons.cpp
  src/FreeSpaceLoss.cpp
  src/FresnelIntegral.cpp
  src/H0Function.cpp
  src/InitializeArea.cpp
  src/InitializePointToPoint.cpp
  src/InverseComplementaryCumulativeDistributionFunction.cpp
  src/itm_area.cpp
  src/itm_p2p.cpp
  src/KnifeEdgeDiffraction.cpp
  src/LinearLeastSquaresFit.cpp
  src/LineOfSightLoss.cpp
  src/LongleyRice.cpp
  src/QuickPfl.cpp
  src/SigmaHFunction.cpp
  src/SmoothEarthDiffraction.cpp
  src/TerrainRoughness.cpp
  src/TroposcatterLoss.cpp
  src/ValidateInputs.cpp
  src/Variability.cpp
)
target_include_directories(ITModel PUBLIC  
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include/ITModel>"
)

add_executable(RunITModel RunITModel.cpp)
target_link_libraries(RunITModel ITModel)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/ITModelConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/ITModelConfig.cmake"
    INSTALL_DESTINATION lib/cmake/ITModel
)


install(FILES
        "${PROJECT_BINARY_DIR}/ITModelConfig.cmake"
        DESTINATION lib/cmake/ITModel)

install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/include/Enums.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/Errors.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/itm.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/Warnings.h"
        DESTINATION include/ITModel
        )

install(TARGETS ITModel RunITModel
  EXPORT ITModelTargets
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include
  INCLUDES DESTINATION include
)

install(EXPORT ITModelTargets DESTINATION lib/cmake/ITModel)
