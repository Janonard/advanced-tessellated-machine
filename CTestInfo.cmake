enable_testing()

macro(atm_add_test TESTNAME)
	add_test(NAME "${TESTNAME}" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/" COMMAND atm -r "${TESTNAME}.atma")
endmacro()

atm_add_test(halt)
atm_add_test(directMemoryAccess)
atm_add_test(indirectMemoryAccess)
atm_add_test(hello_world)
