# installing
install(TARGETS atm DESTINATION bin)
install(FILES LICENSE DESTINATION share/doc/atm/)
install(DIRECTORY share/ DESTINATION share)

# general information
set(CPACK_GENERATOR "RPM;TGZ")
set(CPACK_PACKAGE_NAME "atm")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "assembler programming suite")
set(CPACK_PACKAGE_VENDOR "Janonard")
set(CPACK_PACKAGE_CONTACT "Janonard <janonard@protonmail.com>")
set(CPACK_PACKAGE_VERSION_MAJOR "${ATM_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${ATM_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${ATM_VERSION_PATCH}")

# license and descripton files
set(CPACK_RESOURCE_FILE_LICENSE  "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

# rpm-specific info
set(CPACK_RPM_PACKAGE_LICENSE "GPLv3")
set(CPACK_RPM_PACKAGE_REQUIRES "glfw, SOIL, libGLEW, boost-filesystem")

# deb-specific info
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libglfw3, libsoil1, libglew1.13, libboost-filesystem1.61.0")
set(CPACK_DEBIAN_FILE_NAME ${DEB-DEFAULT})

include(CPack)
