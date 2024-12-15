{
	inputs = {
		nixpkgs.url = "github:nixos/nixpkgs/18536bf04cd71abd345f9579158841376fdd0c5a";
		flake-utils.url = "github:numtide/flake-utils";
	};
	outputs = { self, nixpkgs, flake-utils }:
		flake-utils.lib.eachDefaultSystem (system: 
			let pkgs = nixpkgs.legacyPackages.${system};
			in {
				devShell = pkgs.mkShell {
					buildInputs = with pkgs; [
						#codeblocksFull
						freeglut
						mesa
						libGL
						libGLU
						#glew
						#SDL2
						#SDL2_image
						#stb
						#freetype
						#ventoy-full
					];
					shellHook = ''
						echo ${pkgs.freeglut.dev}/include/GL
					'';
				};
				packages.default = pkgs.stdenv.mkDerivation {
					name = "glTest";
					src = ./curvSubPar.cpp;
					buildInputs = with pkgs; [
						freeglut
						libGL
						libGLU
						#glew
						#SDL2
						#SDL2_image
						#stb
						#freetype
					];
					#includeDir = ./include;
					#include1 = ./include/glm.h;
					#include2 = ./include/glm.cpp;
					#include3 = ./include/stdafx.h;
					#include4 = ./include/stdafx.cpp;
					#include5 = ./include/Texture.h;
					#include6 = ./include/Texture.cpp;
					# $include1 $include2 $include3 $include4 $include5 $include6

					dontUnpack = true;
					buildPhase = ''
						ls
						echo $src
						mkdir -p $out/bin
						g++ $src -o $out/bin/$name -lGL -lGLU -lglut 
						
					'';
				};

				packages.help  = pkgs.stdenv.mkDerivation {
					name = "help";
					src = ./help.cpp;
					buildInputs = with pkgs; [
						# freeglut
						# libGL
						# libGLU
						#glew
						#SDL2
						#SDL2_image
						#stb
						#freetype
					];
					#includeDir = ./include;
					#include1 = ./include/glm.h;
					#include2 = ./include/glm.cpp;
					#include3 = ./include/stdafx.h;
					#include4 = ./include/stdafx.cpp;
					#include5 = ./include/Texture.h;
					#include6 = ./include/Texture.cpp;
					# $include1 $include2 $include3 $include4 $include5 $include6

					dontUnpack = true;
					buildPhase = ''
						ls
						echo $src
						mkdir -p $out/bin
						g++ $src -o $out/bin/$name
					'';
				};

				packages.oldcode = pkgs.stdenv.mkDerivation {
					name = "oldcode";
					src = ./oldcode.cpp;
					buildInputs = with pkgs; [
						freeglut
						libGL
						libGLU
						#glew
						#SDL2
						#SDL2_image
						#stb
						#freetype
					];
					#includeDir = ./include;
					#include1 = ./include/glm.h;
					#include2 = ./include/glm.cpp;
					#include3 = ./include/stdafx.h;
					#include4 = ./include/stdafx.cpp;
					#include5 = ./include/Texture.h;
					#include6 = ./include/Texture.cpp;
					# $include1 $include2 $include3 $include4 $include5 $include6

					dontUnpack = true;
					buildPhase = ''
						ls
						echo $src
						mkdir -p $out/bin
						g++ $src -o $out/bin/$name -lGL -lGLU -lglut 
						
					'';
				};
			}
		);
}
