target_sources (${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spu_dump.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/spu_utilities.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/spu.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../${CMAKE_STACK_DIR}/source/log.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../${CMAKE_STACK_DIR}/source/stack_utilities.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../${CMAKE_STACK_DIR}/source/stack.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../${CMAKE_ONEGIN_DIR}/source/Onegin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../${CMAKE_ONEGIN_DIR}/source/utilities.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../${CMAKE_ONEGIN_DIR}/source/Sorting.cpp
)
