target_sources (${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/asm_dump.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/asm_utilities.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/assembler.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/get_cmd_arg.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/labels.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../${CMAKE_ONEGIN_DIR}/source/Onegin.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../${CMAKE_ONEGIN_DIR}/source/utilities.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../${CMAKE_ONEGIN_DIR}/source/Sorting.cpp
)
