# 
# Try to find DevIL (developer image) library.
# http://openil.sourceforge.net/
#
# Once run this will define: 
# 
# DEVIL_FOUND
# DEVIL_INCLUDE_DIR
# DEVIL_LIBRARIES
#
# Jan Woetzel 12/2005. 
# 
# www.mip.informatik.uni-kiel.de/~jw
# --------------------------------

SET(DEVIL_POSSIBLE_INC_PATHS
  $ENV{DEVIL_DIR}/include
  $ENV{DEVIL_DIR}
  $ENV{DEVIL_HOME}/include 
  $ENV{DEVIL_HOME}
  $ENV{SOURCE_DIR}/DevIL/include
  $ENV{SOURCE_DIR}/DevIL
  $ENV{SOURCE_DIR}
  $ENV{SOURCE_DIR}/include
  /usr/local/include
  /usr/include
  /opt/net/gcc41/DevIL/include
  /opt/net/gcc33/DevIL/include  
  )

SET(DEVIL_POSSIBLE_LIB_PATHS
  $ENV{DEVIL_DIR}/lib
  $ENV{DEVIL_DIR}
  $ENV{DEVIL_HOME}/lib
  $ENV{DEVIL_HOME}
  $ENV{SOURCE_DIR}/DevIL/lib
  $ENV{SOURCE_DIR}/DevIL
  $ENV{SOURCE_DIR}
  $ENV{SOURCE_DIR}/lib
  /usr/local/lib
  /usr/lib
  /opt/net/gcc41/DevIL/lib
  /opt/net/gcc33/DevIL/lib  
  )


FIND_PATH(DEVIL_INCLUDE_DIR IL/il.h
  ${DEVIL_POSSIBLE_INC_PATHS} )
#MESSAGE("DBG DEVIL_INCLUDE_DIR=${DEVIL_INCLUDE_DIR}")  



FIND_LIBRARY(DEVIL_LIBRARY_IL
  NAMES DevIL devil DevIL IL
  PATHS ${DEVIL_POSSIBLE_LIB_PATHS} )
#MESSAGE("DBG DEVIL_LIBRARY_IL=${DEVIL_LIBRARY_IL}")



FIND_LIBRARY(DEVIL_LIBRARY_ILU
  NAMES ILU ilu Ilu 
  PATHS ${DEVIL_POSSIBLE_LIB_PATHS} )
#MESSAGE("DBG DEVIL_LIBRARY_ILU=${DEVIL_LIBRARY_ILU}")



FIND_LIBRARY(DEVIL_LIBRARY_ILUT
  NAMES ILUT ilu Ilut 
  PATHS ${DEVIL_POSSIBLE_LIB_PATHS}  )
#MESSAGE("DBG DEVIL_LIBRARY_ILUT=${DEVIL_LIBRARY_ILUT}")



# --------------------------------

IF  (DEVIL_INCLUDE_DIR)
  IF  (DEVIL_LIBRARY_IL)
    IF  (DEVIL_LIBRARY_ILU AND DEVIL_LIBRARY_ILUT)

      SET(DEVIL_FOUND TRUE)
      SET(DEVIL_LIBRARIES 
	${DEVIL_LIBRARY_IL}
	${DEVIL_LIBRARY_ILU}
	${DEVIL_LIBRARY_ILUT} )

      # get the link directory for rpath to be used with LINK_DIRECTORIES: 	 
      GET_FILENAME_COMPONENT(DEVIL_LINK_DIRECTORIES ${DEVIL_LIBRARY_IL} PATH)  

    ENDIF(DEVIL_LIBRARY_ILU AND DEVIL_LIBRARY_ILUT)
  ENDIF(DEVIL_LIBRARY_IL)
ENDIF  (DEVIL_INCLUDE_DIR)


MARK_AS_ADVANCED(
  DEVIL_INCLUDE_DIR
  DEVIL_LIBRARY_IL
  DEVIL_LIBRARY_ILU
  DEVIL_LIBRARY_ILUT
  DEVIL_LIBRARIES
  )
