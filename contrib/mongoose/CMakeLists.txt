FILE(GLOB_RECURSE mongoose_SOURCES "*.c")
#aux_source_directory(${CMAKE_CURRENT_LIST_DIR})
#SET(mongoose_SOURCES ${CMAKE_CURRENT_LIST_DIR})

# Mongoose does not compile with strict aliasing.
# 禁用警告转错误
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing")
IF(MG_ENABLE_DEBUG)
ADD_DEFINITIONS(-DMG_ENABLE_DEBUG)
ELSE()
  ADD_DEFINITIONS(-DMG_DISABLE_STDIO)
ENDIF()
                                                                                
ADD_LIBRARY(mongoose_static STATIC ${mongoose_SOURCES})
if(CMAKE_COMPILER_IS_GNUCC)
  target_compile_options(mongoose_static PRIVATE -Wno-error)
endif()

SET_TARGET_PROPERTIES(mongoose_static PROPERTIES OUTPUT_NAME mongoose)
TARGET_LINK_LIBRARIES(mongoose_static)
INSTALL(TARGETS mongoose_static DESTINATION lib)

IF(ENABLE_SHARED_LIBS)
  ADD_LIBRARY(mongoose_shared SHARED ${mongoose_SOURCES})
  if(CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(mongoose_shard PRIVATE -Wno-error)
  endif()

  SET_TARGET_PROPERTIES(mongoose_shared PROPERTIES OUTPUT_NAME mongoose)
  TARGET_LINK_LIBRARIES(mongoose_shared)
  INSTALL(TARGETS mongoose_shared DESTINATION lib)
ENDIF()
