add_executable(${EXE_NAME}
    src/GameWorld.cpp
)
target_link_libraries(${EXE_NAME} PRIVATE ${ENGINE_NAME})