workspace "The Forgotten Server"
   configurations { "Debug", "Release"}
   platforms { "64", "ARM64" }
   location ""
   editorintegration "On"

   project        "The Forgotten Server"
      kind        "ConsoleApp"
      language    "C++"
      cppdialect  "C++20"
      targetdir   "%{wks.location}"
      objdir      "build/%{cfg.buildcfg}/obj"
      location    ""
      files { "src/**.cpp", "src/**.h" }
      removefiles {"src/tests/*.cpp", "src/tests/*.h", "src/http/tests/*.cpp", "src/http/test/*.h"}
      flags {"LinkTimeOptimization", "MultiProcessorCompile"}
      enableunitybuild "On"
      intrinsics   "On"

      filter "configurations:Debug"
         defines { "DEBUG" }
         symbols "On"
         optimize "Debug"
      filter {}

      filter "configurations:Release"
         defines { "NDEBUG" }
         symbols "On"
         optimize "Speed"
      filter {}

      filter "platforms:64"
         architecture "x86_64"
      filter {}

      filter "platforms:ARM64"
         architecture "ARM64"
      filter {}

      filter "system:not windows"
         buildoptions { "-Wall", "-Wextra", "-pedantic", "-pipe", "-fvisibility=hidden", "-Wno-unused-local-typedefs" }
         linkoptions{"-flto=auto"}
      filter {}

      filter "system:windows"
         openmp "On"
         characterset "MBCS"
         debugformat "c7"
         linkoptions {"/IGNORE:4099"}
         vsprops { VcpkgEnableManifest = "true" }
      filter {}

      ilter "architecture:amd64"
        vectorextensions "AVX"
      filter{}

      filter { "system:linux", "architecture:ARM64" }
         -- Paths to vcpkg manifest installed dependencies
         libdirs { "vcpkg_installed/arm64-linux/lib" }
         includedirs { "vcpkg_installed/arm64-linux/include" }
         links { "pugixml", "lua", "fmt", "ssl", "mariadb", "cryptopp", "crypto", "boost_iostreams", "zstd", "z" }
      filter{}

      filter { "system:linux", "architecture:amd64" }
         -- Paths to vcpkg manifest installed dependencies
         libdirs { "vcpkg_installed/x64-linux/lib" }
         includedirs { "vcpkg_installed/x64-linux/include" }
         links { "pugixml", "lua", "fmt", "ssl", "mariadb", "cryptopp", "crypto", "boost_iostreams", "zstd", "z" }
      filter{}

      filter "toolset:gcc"
         buildoptions { "-fno-strict-aliasing" }
         buildoptions {"-std=c++20"}
      filter {}

      filter "toolset:clang"
         buildoptions { "-Wimplicit-fallthrough", "-Wmove" }
      filter {}

      filter { "system:macosx", "action:gmake" }
         buildoptions { "-fvisibility=hidden" }   
      filter {}