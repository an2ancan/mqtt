cmake_minimum_required(VERSION 3.16)
project(publisher)
find_package(PahoMqttCpp REQUIRED)

include_directories(
    include
    include/${PROJECT_NAME}
    src
    SYSTEM
    ../topic/include
)

set(SOURCE_FILES
    src/bicycle_model.cpp
    include/publisher/bicycle_model.hpp
    src/publisher.cpp
    include/publisher/publisher.hpp
    include/publisher/callback.hpp
)
add_library(${PROJECT_NAME}_lib SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}_lib 
    PahoMqttCpp::paho-mqttpp3-static 
    ncurses 
)
target_include_directories(
        ${PROJECT_NAME}_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}"
)

add_executable( ${PROJECT_NAME} 
    entry_point/${PROJECT_NAME}_main.cpp 
)

target_link_libraries(${PROJECT_NAME} 
    PahoMqttCpp::paho-mqttpp3-static 
    ncurses 
    ${PROJECT_NAME}_lib
)

install(TARGETS ${INSTALL_TARGETS} EXPORT ${PROJECT_NAME} 
	RUNTIME DESTINATION bin
)
install(TARGETS ${INSTALL_TARGETS} EXPORT ${PROJECT_NAME}_lib 
	LIBRARY DESTINATION lib
)