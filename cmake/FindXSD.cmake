# 
# Attempt to find the xsd application in various places. If found, the full
# path will be in XSD_EXECUTABLE. Look in the usual locations, as well as in
# the 'bin' directory in the path given in the XSD_ROOT environment variable.
#

# Modified version of Brad Howes' (howes@ll.mit.edu) FindXSD.cmake file
# Taken from XSD user's mailing list
# Licensing is uncertain
# TODO ask for definite licensing on this piece of code


# Addition: TRY_XSD_EXECUTABLE
# TRY_XSD_EXECUTABLE
# Used to diambiguate between Mono's xsd and xsdcxx on Ubuntu
# EXEC_NAME - The command line name of the executable
FUNCTION(TRY_XSD_EXECUTABLES XSD_EXECUTABLE_NAME XSD_VERSION_NAME)
  FOREACH(EXEC_NAME IN LISTS ARGN)
    UNSET(XSD_EXEC CACHE)
    FIND_PROGRAM(XSD_EXEC NAMES ${EXEC_NAME}
		   	  HINTS ${RWSL_DEPS}/xsd/bin $ENV{XSD_ROOT}/bin
			  PATHS /usr/local/xsd-4.0.0-i686-macosx/bin
			  		/usr/local/xsd-4.0.0-x86_64-linux-gnu/bin
			  		/usr/local/bin
					/opt/xsd-4.0.0-i686-macosx/bin
			  		/opt/xsd-4.0.0-x86_64-linux-gnu/bin
			  		/usr/bin
					ENV PATH )
    IF(XSD_EXEC)
      # Ensure this isn't Mono's xsd
      # Do this by examining the output from version
      EXECUTE_PROCESS(COMMAND ${XSD_EXEC} version RESULT_VARIABLE OUTCODE OUTPUT_VARIABLE OUTDATA ERROR_VARIABLE ERRDATA)
      
      # Try to find CodeSynthesis info in the stderr data
      STRING(FIND "${ERRDATA}" CodeSynthesis CS_ERR_LOCATION)
      # Try to find CodeSynthesis info in the stdout data
      STRING(FIND "${OUTDATA}" CodeSynthesis CS_OUT_LOCATION)
      IF(CS_ERR_LOCATION EQUAL 0)
        # Older versions use stderr
        SET(VERSIONDATA "${ERRDATA}")
        SET(CS_LOCATION 0)
      ELSEIF(CS_OUT_LOCATION EQUAL 0)
        # Newer versions use stdout
        SET(VERSIONDATA "${OUTDATA}")
        SET(CS_LOCATION 0)
      ELSE(CS_ERR_LOCATION EQUAL 0)
        # Couldn't find it in either location
        UNSET(XSD_EXEC CACHE)
        CONTINUE()
      ENDIF(CS_ERR_LOCATION EQUAL 0)
      
      # Match to find the version
      STRING(REGEX MATCH "[0-9].[0-9].[0-9]" XSD_VERSION "${VERSIONDATA}")
      MESSAGE(STATUS "Using XSD version ${XSD_VERSION}")
      
      IF(CS_LOCATION EQUAL 0)
        BREAK()
      ELSE(CS_LOCATION EQUAL 0)
      ENDIF(CS_LOCATION EQUAL 0)
    ENDIF(XSD_EXEC)
  ENDFOREACH(EXEC_NAME IN ARGN)
  SET(${XSD_EXECUTABLE_NAME} "${XSD_EXEC}" PARENT_SCOPE)
ENDFUNCTION(TRY_XSD_EXECUTABLES)

TRY_XSD_EXECUTABLES(XSD_EXECUTABLE XSD_VERSION xsdcxx xsd)

IF( XSD_EXECUTABLE )

  # 
  # Obtain the include directory that one can use with INCLUDE_DIRECTORIES() to
  # access the xsd include files.
  #
  GET_FILENAME_COMPONENT( XSD_BIN_DIR "${XSD_EXECUTABLE}" PATH )
  GET_FILENAME_COMPONENT( XSD_ROOT_DIR "${XSD_BIN_DIR}" PATH )
  SET( XSD_INCLUDE_DIR "${XSD_ROOT_DIR}/libxsd" )
ENDIF( XSD_EXECUTABLE )

#
# General CMake package configuration.
#
INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( XSD DEFAULT_MSG XSD_EXECUTABLE
								   XSD_INCLUDE_DIR )
IF( XSD_FOUND )
  SET( XSD_INCLUDE_DIRS ${XSD_INCLUDE_DIR} )
ELSE( XSD_FOUND )
  SET( XSD_INCLUDE_DIRS )
ENDIF( XSD_FOUND )

MARK_AS_ADVANCED( XSD_INCLUDE_DIR XSD_EXECUTABLE )

# Addition: XSD_LIBRARIES
# Requires XercesC for compilation
FIND_PACKAGE(XercesC)

IF(XERCESC_FOUND)
  SET( XSD_LIBRARY ${XERCESC_LIBRARY} )
  SET( XSD_LIBRARIES ${XSD_LIBRARY} )
ELSE(XERCESC_FOUND)
  SET( XSD_FOUND FALSE )
ENDIF(XERCESC_FOUND)

# 
# Function that attempts to generate C++ files from an XML schema. The NAME
# argument is the name of the CMake variable to use to store paths to the
# derived C++ source file. The FILE argument is the path of the schema file to
# process. Additional arguments should be XSD command-line options.
#
# Example:
#
# XSD_SCHEMA( FOO_SRCS Foo.xsd --root-element-first --generate-serialization )
#
# On return, FOO_SRCS will contain Foo.cxx.
#
FUNCTION( XSD_SCHEMA NAME FILE )

  #
  # Make a full path from the source directory
  #
  SET( xs_SRC "${FILE}" )

  # 
  # XSD will generate two or three C++ files (*.cxx,*.hxx,*.ixx). Get the
  # destination file path sans any extension and then build paths to the
  # generated files.
  #
  GET_FILENAME_COMPONENT( xs_FILE "${FILE}" NAME_WE )
  SET( xs_CXX "${CMAKE_CURRENT_BINARY_DIR}/${xs_FILE}.cxx" )
  SET( xs_HXX "${CMAKE_CURRENT_BINARY_DIR}/${xs_FILE}.hxx" )
  SET( xs_IXX "${CMAKE_CURRENT_BINARY_DIR}/${xs_FILE}.ixx" )
  
  #
  # Add the source files to the NAME variable, which presumably will be used to
  # define the source of another target.
  #
  LIST( APPEND ${NAME} ${xs_CXX} )
  SET(${NAME} "${${NAME}}" PARENT_SCOPE)
  
  SET(${NAME}_HEADER "${xs_HXX}" PARENT_SCOPE)

  #
  # Set up a generator for the output files from the given schema file using
  # the XSD cxx-tree command.
  #
  ADD_CUSTOM_COMMAND( OUTPUT "${xs_CXX}" "${xs_HXX}" "${xs_IXX}"
    COMMAND ${XSD_EXECUTABLE}
    ARGS "cxx-tree" "--output-dir" "${CMAKE_CURRENT_BINARY_DIR}" ${ARGN} "${xs_SRC}"
    DEPENDS ${xs_SRC} )

  #
  # Don't fail if a generated file does not exist.
  # Needs to be repeated
  #
  SET_SOURCE_FILES_PROPERTIES( "${xs_CXX}" "${xs_HXX}" "${xs_IXX}"
    PROPERTIES GENERATED TRUE )

ENDFUNCTION( XSD_SCHEMA )
