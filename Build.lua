-- premake5.lua
workspace "Crypt Chat"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Client"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
	include "Core/Build-Core.lua"
group ""

include "Client/Build-Client.lua"
include "Server/Build-Server.lua"