cmake_minimum_required(VERSION 3.2)

project(server)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    src/server.cpp
    src/session.cpp
)

#Liens avec les librairies boost
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    boost_system
    pthread
)