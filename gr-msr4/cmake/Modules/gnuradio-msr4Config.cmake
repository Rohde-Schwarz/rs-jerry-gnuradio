find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_MSR4 gnuradio-msr4)

FIND_PATH(
    GR_MSR4_INCLUDE_DIRS
    NAMES gnuradio/msr4/api.h
    HINTS $ENV{MSR4_DIR}/include
        ${PC_MSR4_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_MSR4_LIBRARIES
    NAMES gnuradio-msr4
    HINTS $ENV{MSR4_DIR}/lib
        ${PC_MSR4_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-msr4Target.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_MSR4 DEFAULT_MSG GR_MSR4_LIBRARIES GR_MSR4_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_MSR4_LIBRARIES GR_MSR4_INCLUDE_DIRS)
