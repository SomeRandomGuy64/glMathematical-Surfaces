add_rules("mode.debug", "mode.release")

-- Specify the required packages
add_requires("glfw", {alias = "glfw"})
add_requires("opengl")

-- Define the target (your project)
target("glMathematical-Surfaces")
    set_kind("binary")  -- or 'static', 'shared', etc.
    set_languages("c++20")

    -- Set the include directories
    add_includedirs("src/Header Files")
    add_includedirs("src/Includes")
    add_includedirs("src/Shaders")

    -- Add source files
    add_files("src/*.cpp")
    add_files("src/*.c")

    -- Link against OpenGL (system library)
    add_syslinks("opengl32")  -- For Windows
    -- add_syslinks("GL")  -- For Linux or macOS

    -- Link against GLFW (using the alias we set earlier)
    add_packages("glfw")

