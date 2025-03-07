cmake_minimum_required (VERSION 3.5)

project (genTerrain)

set(CMAKE_CXX_STANDARD 11)

include_directories("include"
                    "thirdParties"
                    "thirdParties/FastNoise"
                    "thirdParties/Eigen"
                    "thirdParties/ImGui"
                    ${GLFW_INCLUDE_DIRS})

set(imgui_sources
    "thirdParties/ImGui/imgui.cpp"
    "thirdParties/ImGui/imgui_widgets.cpp"
    "thirdParties/ImGui/imgui_draw.cpp"
    "thirdParties/ImGui/imgui_demo.cpp"
    "thirdParties/ImGui/imgui_impl_glfw.cpp"
    "thirdParties/ImGui/imgui_impl_opengl3.cpp")

set(fnoise_sources
    "thirdParties/FastNoise/FastNoise.cpp")

set(sources
    "src/Box2d.cpp"
    "src/Grid2d.cpp"
    "src/DoubleField.cpp"
    "src/SimpleLayerMap.cpp"
    "src/MultiLayerMap.cpp"
    "src/Noise/TerrainNoise.cpp"
    "src/Weather/Erosion.cpp"
    "src/Weather/Hydro.cpp"
    "src/Weather/Biome.cpp"
    "src/Vegetation/Vegetation.cpp"
    "src/Vegetation/VegetationLayerMap.cpp"
    "src/Vegetation/Plant/Grass.cpp"
    "src/Vegetation/Plant/Bush.cpp"
    "src/Vegetation/Plant/Tree.cpp"
    "src/Utils.cpp")

set(test_sources
    "src/tests/test_Box2d.cpp"
    "src/tests/test_Grid2d.cpp"
    "src/tests/test_SimpleLayerMap.cpp")

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_library(terrain STATIC ${sources})
target_link_libraries(terrain fnoise)
add_library(imgui STATIC ${imgui_sources})
target_link_libraries(imgui ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw)
add_library(fnoise STATIC ${fnoise_sources})


add_executable(genTerrainGraphique
    "src/mainGraph.cpp")
target_link_libraries(genTerrainGraphique ${GLEW_LIBRARIES} glfw imgui fnoise terrain)

add_executable(genTerrain
    "src/main.cpp")
target_link_libraries(genTerrain fnoise terrain)


add_executable(tests
    ${test_sources})
target_link_libraries(tests fnoise terrain)
