add_executable(${PROJECT_NAME} 
    src/GameWorld.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE EriEngine)
