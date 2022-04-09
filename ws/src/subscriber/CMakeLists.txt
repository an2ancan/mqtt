cmake_minimum_required(VERSION 3.16)
project(subscriber)
find_package(PahoMqttCpp REQUIRED)

include_directories(
    include
    include/${PROJECT_NAME}
    src
    SYSTEM
    ../topic/include
)

set(SOURCE_FILES
    src/callback.cpp
    include/${PROJECT_NAME}/action_listner.hpp
    include/${PROJECT_NAME}/callback.hpp
    include/${PROJECT_NAME}/diff_accumulator.hpp
)
add_library(${PROJECT_NAME}_lib SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}_lib 
    PahoMqttCpp::paho-mqttpp3-static 
)
target_include_directories(
        ${PROJECT_NAME}_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}"
)

add_executable( ${PROJECT_NAME} 
    entry_point/${PROJECT_NAME}_main.cpp 
)

target_link_libraries(${PROJECT_NAME} 
    PahoMqttCpp::paho-mqttpp3-static 
    ${PROJECT_NAME}_lib
)

install(TARGETS ${INSTALL_TARGETS} EXPORT ${PROJECT_NAME} 
	RUNTIME DESTINATION bin
)
install(TARGETS ${INSTALL_TARGETS} EXPORT ${PROJECT_NAME}_lib 
	LIBRARY DESTINATION lib
)