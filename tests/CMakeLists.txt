
file(GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )
file(GLOB H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h )

set(TEST_FILES ${CPP_FILES} ${C_FILES} ${H_FILES})

if (EXTRA_INCLUDE_DIRS) 
 include_directories(${EXTRA_INCLUDE_DIRS})
endif(EXTRA_INCLUDE_DIRS)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
add_executable(test_api ${TEST_FILES})
set_target_properties(test_api PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_link_libraries(test_api cpsapi ${LIBCOPASISE_LIBRARY} ${EXTRA_LIBS})
add_test(NAME test_api_run COMMAND "$<TARGET_FILE:test_api>")
set_tests_properties(test_api_run PROPERTIES ENVIRONMENT 
	"srcdir=${CMAKE_CURRENT_SOURCE_DIR}")