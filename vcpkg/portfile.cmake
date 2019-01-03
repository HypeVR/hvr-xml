include(vcpkg_common_functions)

set(VCPKG_LIBRARY_LINKAGE dynamic)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO HypeVR/hvr-xml
    REF 0.1.13
    SHA512 5ff0fedda60446fb181314c319dde72ef82ceeab43c8033451b36ea7befe7dcb5a71ce4c8c9a0798212792f28b98cab8ec7a0e33b4140554445d3106f88d6be3
    HEAD_REF master
)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
)

vcpkg_install_cmake()

vcpkg_fixup_cmake_targets(CONFIG_PATH "lib/cmake/hvr-xml")

vcpkg_copy_pdbs()

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)
file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/share)
file(GLOB EXE ${CURRENT_PACKAGES_DIR}/bin/*.exe)
file(GLOB DEBUG_EXE ${CURRENT_PACKAGES_DIR}/debug/bin/*.exe)
file(COPY ${EXE} DESTINATION ${CURRENT_PACKAGES_DIR}/tools/hvr-xml/)
file(REMOVE ${EXE})
file(REMOVE ${DEBUG_EXE})

file(COPY ${SOURCE_PATH}/README.md DESTINATION ${CURRENT_PACKAGES_DIR}/share/hvr-xml/)
file(RENAME ${CURRENT_PACKAGES_DIR}/share/hvr-xml/README.md ${CURRENT_PACKAGES_DIR}/share/hvr-xml/copyright)
