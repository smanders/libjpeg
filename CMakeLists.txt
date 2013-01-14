# This package contains programs for manipulating JPEG files:
#
# cjpeg/djpeg: convert to/from the JPEG file format
# rdjpgcom/wrjpgcom: read/write comments in JPEG files
# jpegtran: lossless transformations of JPEG files
# jpegexiforient/exifautotran: manipulate EXIF orientation tag
# jpegdump (jpeginfo like tool), simply dump more details

IF(NOT LJPEG_NAMESPACE)
  SET(LJPEG_NAMESPACE "LJPEG")
  SET(LJPEG_STANDALONE 1)
  SET(LJPEG_INSTALL_BIN_DIR bin)
  SET(LJPEG_INSTALL_LIB_DIR lib)
  SET(LJPEG_INSTALL_INCLUDE_DIR include/ljpeg)
ENDIF(NOT LJPEG_NAMESPACE)
# In all cases:
STRING(TOLOWER ${LJPEG_NAMESPACE} LJPEG_LIBRARY_NAME)

PROJECT(${LJPEG_NAMESPACE} C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

# Do full dependency headers.
INCLUDE_REGULAR_EXPRESSION("^.*$")

#-----------------------------------------------------------------------------
# Disallow insource build since I never test that
STRING(COMPARE EQUAL "${LJPEG_SOURCE_DIR}" "${LJPEG_BINARY_DIR}" INSOURCE)
GET_FILENAME_COMPONENT(PARENTDIR ${LJPEG_BINARY_DIR} PATH)
STRING(COMPARE EQUAL "${LJPEG_SOURCE_DIR}" "${PARENTDIR}" INSOURCESUBDIR)
IF(INSOURCE OR INSOURCESUBDIR)
  MESSAGE(FATAL_ERROR "LJPEG requires an out of source Build. "
    "Please create a separate binary directory and run CMake there.")
ENDIF(INSOURCE OR INSOURCESUBDIR)

#-----------------------------------------------------------------------------
# IJG version number, usefull for packaging and doxygen doc:
SET(LJPEG_MAJOR_VERSION 62) # 62 number version comes from the debian shared lib: /usr/lib/libjpeg.so.62
SET(LJPEG_MINOR_VERSION 1)  # bump up the minor version number so that there is no collision
SET(LJPEG_BUILD_VERSION 0)
SET(LJPEG_VERSION
  "${LJPEG_MAJOR_VERSION}.${LJPEG_MINOR_VERSION}.${LJPEG_BUILD_VERSION}")

SET(LJPEG_LIBRARY_PROPERTIES ${LJPEG_LIBRARY_PROPERTIES}
  VERSION "${LJPEG_VERSION}"
  SOVERSION "${LJPEG_MAJOR_VERSION}.${LJPEG_MINOR_VERSION}"
)


# memmgr back ends: compile only one of these into a working library
# (For now, let's use the mode that requires the image fit into memory.
# This is the recommended mode for Win32 anyway.)
SET(systemdependent_SRCS jmemnobs.c)

# library object files common to compression and decompression
SET(common_SRCS
jcomapi.c jutils.c jerror.c jmemmgr.c
)

# compression library object files
SET(compression_SRCS
jcapimin.c jcapistd.c jctrans.c jcparam.c  jcinit.c
jcmaster.c jcmarker.c jcmainct.c jcprepct.c jccoefct.c jccolor.c
jcsample.c jchuff.c jcphuff.c jcdctmgr.c jfdctfst.c jfdctflt.c
jdatadst.c
)

# Lossy (DCT) codec:
SET(comp_lossy_SRCS
jfdctint.c
jclossy.c

# lossless
jclossls.c

#jdarith.c
jcodec.c
jcscale.c
jcshuff.c
jclhuff.c
jcpred.c
jcdiffct.c
)

# decompression library object files
SET(decompression_SRCS
jdapimin.c jdapistd.c jdtrans.c  jdmaster.c
jdinput.c jdmarker.c jdhuff.c jdphuff.c jdmainct.c jdcoefct.c
jdpostct.c jddctmgr.c jidctfst.c jidctflt.c jidctint.c jidctred.c
jdsample.c jdcolor.c jquant1.c jquant2.c jdmerge.c
jdatasrc.c
)

SET(decomp_lossy_SRCS
 jdlossls.c jdlossy.c
#jcarith.c
jdlhuff.c
jddiffct.c
jdpred.c
jdscale.c
jdshuff.c
)

SET(JPEG_HEADER_FILES
  jchuff.h  
  jdct.h  
  jdhuff.h  
  jerror.h  
  jinclude.h  
  jmemsys.h  
  jmorecfg.h
  jpegint.h  
  jpeglib.h  
  jversion.h
  jlossy.h
  jlossls.h
  )

SET(LJPEG_SOURCES)
FOREACH(src
  ${systemdependent_SRCS}
  ${common_SRCS}
  ${compression_SRCS}
  ${decompression_SRCS}
  ${comp_lossy_SRCS}
  ${decomp_lossy_SRCS}
  )
  SET(LJPEG_SOURCES ${LJPEG_SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
ENDFOREACH(src)

#-----------------------------------------------------------------------------
# Application
# CJPEG
SET(CJPEG_local_SOURCES
  cjpeg.c
  jdatasrc.c
  rdbmp.c
  rdtarga.c
  rdppm.c
  rdgif.c
  cdjpeg.c
  rdswitch.c
  )
SET(CJPEG_SOURCES)
FOREACH(src ${CJPEG_local_SOURCES})
  SET(CJPEG_SOURCES ${CJPEG_SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
ENDFOREACH(src)
#-----------------------------------------------------------------------------
# DJPEG
SET(DJPEG_local_SOURCES
  djpeg.c
  jdatadst.c
  wrppm.c
  wrgif.c
  cdjpeg.c
  rdcolmap.c
  )

# the following only compile with 8 bits:
SET(DJPEG_local_SOURCES_8
  wrbmp.c
  wrtarga.c
  )
SET(DJPEG_local_SOURCES_12)
SET(DJPEG_local_SOURCES_16)


SET(RDCOLMAP_local_SOURCES
  rdjpgcom.c
)
SET(RDJPGCOM_SOURCES)
FOREACH(src ${RDCOLMAP_local_SOURCES})
  SET(RDJPGCOM_SOURCES ${RDJPGCOM_SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
ENDFOREACH(src)

SET(WRCOLMAP_local_SOURCES
  wrjpgcom.c
)
SET(WRJPGCOM_SOURCES)
FOREACH(src ${WRCOLMAP_local_SOURCES})
  SET(WRJPGCOM_SOURCES ${WRJPGCOM_SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
ENDFOREACH(src)

#-----------------------------------------------------------------------------
# JPEGTRAN
SET(JPEGTRAN_local_SOURCES
  jpegtran.c
  transupp.c
  cdjpeg.c
  rdswitch.c
  )
SET(JPEGTRAN_SOURCES)
FOREACH(src ${JPEGTRAN_local_SOURCES})
  SET(JPEGTRAN_SOURCES ${JPEGTRAN_SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
ENDFOREACH(src)
#-----------------------------------------------------------------------------
SET (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin
  CACHE PATH "Single output directory for building all executables.")
SET (LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin
  CACHE PATH "Single output directory for building all libraries.")
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)


OPTION(BUILD_SHARED_LIBS "Build LJPEG with shared libraries." OFF)

IF (WIN32)
  IF (BUILD_SHARED_LIBS)
    SET(LJPEGDLL 1)
  ELSE (BUILD_SHARED_LIBS)
    SET(LJPEGSTATIC 1)
  ENDIF (BUILD_SHARED_LIBS)
ENDIF (WIN32)


# Generate three libraries: jpeg 8bits, 12bits and 16bits
# and three executable (per libs): djpeg, cjpeg and jpegtran
# using only one source directory


#-----------------------------------------------------------------------------
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
CHECK_INCLUDE_FILE("stddef.h"       HAVE_STDDEF_H)
CHECK_INCLUDE_FILE("stdlib.h"       HAVE_STDLIB_H)

FOREACH(bits 8 12 16)
  #SET(CMAKE_BITS_IN_JSAMPLE ${bits})
SET(DJPEG_SOURCES)
FOREACH(src ${DJPEG_local_SOURCES})
  SET(DJPEG_SOURCES ${DJPEG_SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
ENDFOREACH(src)
FOREACH(src ${DJPEG_local_SOURCES_${bits}})
  SET(DJPEG_SOURCES ${DJPEG_SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
ENDFOREACH(src)

  # Configure file (to be used for a target by CMake)
  CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt.in
    ${CMAKE_CURRENT_BINARY_DIR}/${bits}/CMakeLists.txt
    @ONLY
    )
  # Tell CMake where is the source dir:
  ADD_SUBDIRECTORY(
    ${CMAKE_CURRENT_BINARY_DIR}/${bits}
    ${CMAKE_CURRENT_BINARY_DIR}/${bits}
    )
ENDFOREACH(bits)

# header file
#IF(NOT LJPEG_INSTALL_NO_DEVELOPMENT)
#  FILE(GLOB header_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
#  INSTALL(FILES ${header_files}
#    DESTINATION ${LJPEG_INSTALL_INCLUDE_DIR} COMPONENT Development
#  )
#ENDIF(NOT LJPEG_INSTALL_NO_DEVELOPMENT)

# Install
#INSTALL_FILES(${CMAKE_MAN_DIR}/man1 FILES
#  #${CMAKE_CURRENT_SOURCE_DIR}/ansi2knr.1
#  ${CMAKE_CURRENT_SOURCE_DIR}/cjpeg.1
#  ${CMAKE_CURRENT_SOURCE_DIR}/djpeg.1
#  ${CMAKE_CURRENT_SOURCE_DIR}/jpegdump.1
#  ${CMAKE_CURRENT_SOURCE_DIR}/jpegtran.1
#  ${CMAKE_CURRENT_SOURCE_DIR}/rdjpgcom.1
#  ${CMAKE_CURRENT_SOURCE_DIR}/wrjpgcom.1
#  )

#-----------------------------------------------------------------------------
SET(LJPEG_PACKAGE_DESCRIPTION_SUMMARY "LJPEG - A JPEG lossly+lossless implementation.")
SET(LJPEG_PACKAGE_CONTACT "Mathieu Malaterre <jpeg-users@lists.sourceforge.net>")

# CPack stuff
IF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
  IF(EXISTS "${CMAKE_ROOT}/Modules/InstallRequiredSystemLibraries.cmake")
    SET(CMAKE_INSTALL_MFC_LIBRARIES 0)
    SET(CMAKE_INSTALL_DEBUG_LIBRARIES 0)
    INCLUDE(${CMAKE_ROOT}/Modules/InstallRequiredSystemLibraries.cmake)
  ENDIF(EXISTS "${CMAKE_ROOT}/Modules/InstallRequiredSystemLibraries.cmake")

  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${LJPEG_PACKAGE_DESCRIPTION_SUMMARY})
  SET(CPACK_PACKAGE_VENDOR "LJPEG")
  SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
  SET(CPACK_RESOURCE_FILE_LICENSE    "${CMAKE_CURRENT_SOURCE_DIR}/README")
  SET(CPACK_PACKAGE_VERSION_MAJOR "${LJPEG_MAJOR_VERSION}")
  SET(CPACK_PACKAGE_VERSION_MINOR "${LJPEG_MINOR_VERSION}")
  SET(CPACK_PACKAGE_VERSION_PATCH "${LJPEG_BUILD_VERSION}")
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "LJPEG ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
  SET(CPACK_SOURCE_PACKAGE_FILE_NAME "ljpeg-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

  #SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
  IF(WIN32 AND NOT UNIX)
    #STRING(REGEX REPLACE "/" "\\\\\\\\" CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Release/LJPEGInstall.bmp")

    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backlasshes.
    #SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
    #SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\gdcmviewer.exe")
    SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
    SET(CPACK_NSIS_HELP_LINK "http://jpeg.sourceforge.net")
    SET(CPACK_NSIS_URL_INFO_ABOUT "http://jpeg.sourceforge.net")
    SET(CPACK_NSIS_MODIFY_PATH ON)
    #IF(${CMAKE_INSTALL_PREFIX} MATCHES ${CPACK_PACKAGE_INSTALL_DIRECTORY})
    #ELSE(${CMAKE_INSTALL_PREFIX} MATCHES ${CPACK_PACKAGE_INSTALL_DIRECTORY})
    #STRING(REPLACE ${PROJECT_NAME} ${CPACK_PACKAGE_INSTALL_DIRECTORY} dummy ${CMAKE_INSTALL_PREFIX} )
    #SET(CMAKE_INSTALL_PREFIX ${dummy} CACHE PATH "" FORCE)
    #ENDIF(${CMAKE_INSTALL_PREFIX} MATCHES ${CPACK_PACKAGE_INSTALL_DIRECTORY})
    
  ELSE(WIN32 AND NOT UNIX)
    # http://www.vtk.org/Wiki/CMake:CPackConfiguration
    # List of files to be stripped. Starting with CMake 2.6.0 CPACK_STRIP_FILES will be a boolean variable which enables
    # stripping of all files (a list of files evaluates to TRUE in CMake, so this change is compatible).
    #SET(CPACK_STRIP_FILES "bin/gdcmdump;bin/gdcmconv;bin/gdcmscanner;bin/gdcminfo;bin/gdcmraw;bin/gdcmimg;bin/gdcmtar")
    #SET(CPACK_SOURCE_STRIP_FILES "")
    #SET(CPACK_PACKAGE_EXECUTABLES "gdcmviewer" "VIEWER")

  ENDIF(WIN32 AND NOT UNIX)

  # Need to set the architecture for debian package
  SET(CPACK_PACKAGE_CONTACT ${LJPEG_PACKAGE_CONTACT})
  #SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libstdc++6 (>= 4.0.2-4), libuuid1, zlib1g (>= 1:1.2.1), libgcc1 (>= 1:4.0.2), libexpat1, swig") # bug: missing dep to python...
  #SET(CPACK_DEBIAN_PACKAGE_SUGGESTS "dcmtk")
  SET(CPACK_SOURCE_IGNORE_FILES "/debian/" "/CVS/" "/\\\\.svn/")
  SET(CPACK_IGNORE_FILES ${CPACK_SOURCE_IGNORE_FILES})

  INCLUDE(CPack)
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")


