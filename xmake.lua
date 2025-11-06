set_project(RendererV2)
set_languages("c++23")

add_requires("glfw", "glad", "stb", "tinyobjloader")
add_requires("glm", { alias = "glm" })

target("renderer")
set_kind("binary")
add_files("src/*.cpp")
add_packages("glfw", "glad", "glm", "stb", "tinyobjloader")
add_defines("GLFW_INCLUDE_NONE")

after_build(function(target)
    os.cp("shaders", target:targetdir())
    os.cp("assets", target:targetdir())
end)
