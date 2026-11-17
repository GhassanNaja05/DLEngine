cmake_minimum_required(VERSION 3.24)
project(DLEngine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Raylib supplies windowing, rendering, input and audio. Build a pinned source
# release so no preinstalled library or developer-specific paths are required.
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
FetchContent_Declare(raylib
    URL https://codeload.github.com/raysan5/raylib/tar.gz/refs/tags/5.5
    URL_HASH SHA256=aea98ecf5bc5c5e0b789a76de0083a21a70457050ea4cc2aec7566935f5e258e
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(raylib)

add_library(DLEngine STATIC
    Components/Drawable/Drawable.cpp
    Components/PhysicsBody/PhysicsBody.cpp
    Components/ScriptComponent/Script.cpp
    Components/Shape/CircleShape.cpp
    Components/Shape/RectangleShape.cpp
    Components/Shape/ShapeComponent.cpp
    Components/Sprites/CircleImage.cpp
    Components/Sprites/RectangleImage.cpp
    Components/Sprites/Sprite.cpp
    Components/Sprites/TiledSprite.cpp
    Components/UserInterface/Canvas.cpp
    Components/UserInterface/UserInterface.cpp
    DLCore/Debug/Debug.cpp
    DLCore/Engine/Engine.cpp
    DLCore/EntityManager/EntityManager.cpp
    DLCore/EventManager/EventData.cpp
    DLCore/EventManager/EventManager.cpp
    DLCore/Modules/Audio/Audio.cpp
    DLCore/Modules/Colors/Colors.cpp
    DLCore/Modules/Input/Input.cpp
    DLCore/Modules/Random/Random.cpp
    DLCore/Modules/SpriteManager/SpriteManager.cpp
    DLCore/Modules/Timer/Timer.cpp
    DLCore/PhysicsManager/PhysicsManager.cpp
    GameObject/GameObject.cpp
)
target_include_directories(DLEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(DLEngine PUBLIC raylib)

add_executable(WallJump MACOSX_BUNDLE
    main.cpp
    Components/Tests/WallJump/WallJumpMaker.cpp
    Components/Tests/WallJump/DoudouLapin.cpp
)
target_link_libraries(WallJump PRIVATE DLEngine)

add_executable(FlapBird MACOSX_BUNDLE
    main.cpp
    "Components/Tests/Flap Bird/FlapBirdMaker.cpp"
    "Components/Tests/Flap Bird/Bird.cpp"
    "Components/Tests/Flap Bird/Pillar.cpp"
    "Components/Tests/Flap Bird/PillarSpawner.cpp"
)
target_compile_definitions(FlapBird PRIVATE DLENGINE_FLAP_BIRD)
target_link_libraries(FlapBird PRIVATE DLEngine)

foreach(demo IN ITEMS WallJump FlapBird)
    set_target_properties(${demo} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.dlengine.${demo}"
        MACOSX_BUNDLE_BUNDLE_NAME "${demo}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
    )
endforeach()
