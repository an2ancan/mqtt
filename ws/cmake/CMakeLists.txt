set(package_name Ws)
configure_file(${package_name}Config.cmake.in ${package_name}Config.cmake @ONLY)

export(EXPORT ${package_name}
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${package_name}Targets.cmake"
    NAMESPACE ${package_name}::)

install(EXPORT ${package_name} DESTINATION lib/cmake/${package_name}
    FILE ${package_name}Targets.cmake
    NAMESPACE ${package_name}::)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${package_name}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${package_name}ConfigVersion.cmake"
    DESTINATION lib/cmake/${package_name})
