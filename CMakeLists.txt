cmake_minimum_required(VERSION 3.8)
project(libfluids VERSION 0.1.0 LANGUAGES CXX)

#Make sure that custom modules like FindRapidJSON are found
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

##############################################
# Declare dependencies

find_package(Boost)
find_package(Eigen3 REQUIRED NO_MODULE)

##############################################
# Create target and set properties

set(SRC_FILES
        src/Functor.h
        src/Solver.cpp
        src/System.cpp
        src/Liquid.cpp
        src/FluidComponents.cpp
        src/TransportEdge.h
        src/TransportEdge.cpp
        src/Pipes.cpp
        )

add_library(fluids SHARED ${SRC_FILES})

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(Fluids::fluids ALIAS fluids)

# Set target properties
target_include_directories(fluids
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

target_compile_features(fluids PRIVATE cxx_std_17)
target_compile_options(fluids PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

target_link_libraries(fluids
        PUBLIC
        Boost::boost Eigen3::Eigen)

##############################################
# Installation instructions

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Fluids)

install(TARGETS fluids
        EXPORT fluids-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )

#This is required so that the exported target has the name JSONUtils and not jsonutils
set_target_properties(fluids PROPERTIES EXPORT_NAME Fluids)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#Export the targets to a script
install(EXPORT fluids-targets
        FILE
        FluidsTargets.cmake
        NAMESPACE
        Fluids::
        DESTINATION
        ${INSTALL_CONFIGDIR}
        )

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/FluidsConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
        )

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/FluidsConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/FluidsConfig.cmake
        INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
        )

#Install the config, configversion and custom find modules
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/FluidsConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/FluidsConfigVersion.cmake
        DESTINATION ${INSTALL_CONFIGDIR}
        )

##############################################
## Exporting from the build tree

export(EXPORT fluids-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/FluidsTargets.cmake NAMESPACE Fluids::)

#Register package in user's package registry
export(PACKAGE Fluids)

add_subdirectory(test)

##############################################
## Create package

include(CPack)
