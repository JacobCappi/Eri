add_executable(${PROJECT_NAME} 
    src/entry.cpp
    src/testGame.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE engine)