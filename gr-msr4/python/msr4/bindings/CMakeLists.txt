# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT msr4_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT msr4_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND msr4_python_files
    rs_jerry_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(msr4
   ../../..
   gr::msr4
   "${msr4_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(TARGET msr4_python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:msr4_python>
    ${PROJECT_BINARY_DIR}/test_modules/gnuradio/msr4/
)

install(TARGETS msr4_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/msr4 COMPONENT pythonapi)
