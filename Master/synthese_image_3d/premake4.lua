solution "gKit2light"
    configurations { "debug", "release" }

    platforms { "x64", "x32" }
if _PREMAKE_VERSION >="5.0" then
    platforms { "arm64" }
end
    
    includedirs { ".", "src/gKit" }
    
    gkit_dir = path.getabsolute(".")
    
    configuration "debug"
        targetdir "bin/debug"
        defines { "DEBUG" }
        if _PREMAKE_VERSION >="5.0" then
            symbols "on"
        else
            flags { "Symbols" }
        end
    
    configuration "release"
        targetdir "bin/release"
--~ 		defines { "NDEBUG" }
--~ 		defines { "GK_RELEASE" }
        if _PREMAKE_VERSION >="5.0" then
            optimize "speed"
        else
            flags { "OptimizeSpeed" }
        end
        
    configuration "linux"
        buildoptions { "-mtune=native -march=native" }
        buildoptions { "-std=c++11" }
        buildoptions { "-W -Wall -Wextra -Wsign-compare -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable", "-pipe" }
        links { "GLEW", "SDL2", "SDL2_image", "GL" }
    
    configuration { "linux", "debug" }
        buildoptions { "-g"}
        linkoptions { "-g"}
    
    configuration { "linux", "release" }
        buildoptions { "-fopenmp" }
        linkoptions { "-fopenmp" }
        buildoptions { "-flto"}
        linkoptions { "-flto"}
    
    configuration { "windows", "gmake", "x32" }
        buildoptions { "-U__STRICT_ANSI__"} -- pour definir M_PI
        buildoptions { "-std=c++11", "-fpermissive" }
        defines { "WIN32", "_WIN32" }
        includedirs { "extern/mingw/include" }
        libdirs { "extern/mingw/lib" }
        links { "mingw32", "SDL2main", "SDL2", "SDL2_image", "opengl32", "glew32" }
    
    configuration { "windows", "codeblocks", "x32" }
        buildoptions { "-U__STRICT_ANSI__"} -- pour definir M_PI
        buildoptions { "-std=c++11", "-fpermissive" }
        defines { "WIN32", "_WIN32" }
        includedirs { "extern/mingw/include" }
        libdirs { "extern/mingw/lib" }
        links { "mingw32", "SDL2main", "SDL2", "SDL2_image", "opengl32", "glew32" }
    
if _PREMAKE_VERSION >="5.0" then
    configuration { "windows" }
        defines { "WIN32", "_USE_MATH_DEFINES", "_CRT_SECURE_NO_WARNINGS" }
        defines { "NOMINMAX" } -- allow std::min() and std::max() in vc++ :(((

    configuration { "windows", "vs*" }
        location "build"
        debugdir "."
        
        system "Windows"
        architecture "x64"
        disablewarnings { "4244", "4305" }
        flags { "MultiProcessorCompile", "NoMinimalRebuild" }
        
        includedirs { "extern/visual/include" }
        libdirs { "extern/visual/lib" }
        links { "opengl32", "glew32", "SDL2", "SDL2main", "SDL2_image" }
end
    
    configuration "macosx"
        frameworks= "-F /Library/Frameworks/"
        buildoptions { "-std=c++14 -Wno-deprecated-declarations" }
        defines { "GK_MACOS" }
        buildoptions { frameworks }
        linkoptions { frameworks .. " -framework OpenGL -framework SDL2 -framework SDL2_image" }
    
 -- description des fichiers communs
gkit_files = { gkit_dir .. "/src/gKit/*.cpp", gkit_dir .. "/src/gKit/*.h" }


-- quand ce premake4.lua est inclus par un autre premake qui definit no_project=true (donc quand gkit2light est utilisé comme une lib),
-- ceci stoppe la creation des projects suivants (tuto, etc.)
if no_project then
    do return end
end

 -- description des projets
projects = {
    "shader_kit",
    "shader_kit_debug",
    "image_viewer"
}

for i, name in ipairs(projects) do
    project(name)
        language "C++"
        kind "ConsoleApp"
        targetdir "bin"
        files ( gkit_files )
        files { gkit_dir .. "/src/" .. name..'.cpp' }
end
        
project("gltf")
	language "C++"
	kind "ConsoleApp"
	targetdir "bin"
	files ( gkit_files )
	files { gkit_dir .. "/src/gltf/viewer.cpp" }

project("simple_gltf")
	language "C++"
	kind "ConsoleApp"
	targetdir "bin"
	files ( gkit_files )
	files { gkit_dir .. "/src/gltf/simple.cpp" }

project("M2_SI3D_2")
	language "C++"
	kind "ConsoleApp"
	targetdir "bin"
	files ( gkit_files )
	files { gkit_dir .. "/src/M2_SI3D/tp2_splash.cpp" }

project("M2_SI3D_3")
	language "C++"
	kind "ConsoleApp"
	targetdir "bin"
	files ( gkit_files )
	files { gkit_dir .. "/src/M2_SI3D/tp3.cpp" }
    files { gkit_dir .. "/src/M2_SI3D/utils.h" }
