solution "insolence_toy"
	language "C++"
    objdir "obj"
	targetdir "bin"
	configurations { "Debug", "Release" }

	includedirs {
		"include",
	}

	libdirs {
		"bin",
		"lib"
	}

	links { "insolence"}

	dofile("./src/game.lua")

