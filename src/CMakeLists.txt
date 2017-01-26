include_directories(${PROJECT_SOURCE_DIR}/src/)

macro(atm_include_src FOLDER)
include(src/${FOLDER}/CMakeLists.txt)
endmacro()

atm_include_src(ATM)
atm_include_src(Assembler)
atm_include_src(CPU)
atm_include_src(Executable)
atm_include_src(GraphicsAdapter)
atm_include_src(KeyboardAdapter)
atm_include_src(System)
atm_include_src(TimeAdapter)

add_executable(atm
	${ATM_SRCS}
	${ASSEMBLER_SRCS}
	${CPU_SRCS}
	${EXECUTABLE_SRCS}
	${GRAPHICS_ADAPTER_SRCS}
	${KEYBOARD_ADAPTER_SRCS}
	${SYSTEM_SRCS}
	${TIME_ADAPTER_SRCS}
)
target_link_libraries(atm glfw GLEW GL SOIL ${Boost_SYSTEM_LIBRARY_DEBUG} ${Boost_FILESYSTEM_LIBRARY_DEBUG})
