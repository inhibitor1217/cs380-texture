project(app_project)

file(GLOB APP_SRC "src/*.cpp")
file(GLOB APP_HEADERS "include/*.hpp")

if(MSVC)
    source_group("Header Files" FILES ${APP_HEADERS})

    source_group("Source Files" FILES ${APP_SRC})
endif()

add_executable(App ${APP_SRC} ${APP_HEADERS})

target_include_directories(App
    PUBLIC include
    PRIVATE src 
)

target_link_libraries(App Engine glfw GLEW::GLEW ${OPENGL_LIBRARY} ${ASSIMP_LIBRARIES} glm SOIL)

set_target_properties(
    App PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")