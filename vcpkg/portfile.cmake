include(vcpkg_common_functions)

set(VCPKG_LIBRARY_LINKAGE dynamic)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO HypeVR/hvr-xml
    REF 0.1.16
    SHA512 be241c671bc6b8338afad7dfde6580c808f4a6ec95b9b6c224410bfbd0c4b9d724258317caa7e43525d28d3ff17b1b90a532ebb911fd6223a9d970098f19e10f
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
