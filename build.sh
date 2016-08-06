#!/bin/bash

CONFIGURATION="debug"
OPERATING_SYSTEM=""
RUN_AFTER_BUILD="false"

if [ "$SYSTEMROOT" == "C:\\Windows" ]; then
	PLATFORM="WINDOWS"
else
	PLATFORM="LINUX"
fi

function echo_usage
{
echo "
Usage:
 build.sh [options]

Builds a game project linking insolence.
Requires the insolence shared library files to be manually copied.

Options:
 -r|--run            Run 'insolence_toy'

 -w|--webgl          Build for WebGL
 -R|--release        Release build
 -h|--help           Display this help text
"
}

# $1    Directory to enter, make, and exit out of.
function do_make()
{
	pushd . > /dev/null 2>&1

	cd $1
	echo "Building for $CONFIGURATION"

	if [[ $PLATFORM == "LINUX" ]]; then
		make config="$CONFIGURATION"
	elif [[ $PLATFORM == "WEBGL" ]]; then
		emmake make config="$CONFIGURATION"
	fi

	if [ $? -ne 0 ]; then
		exit
	fi

	popd > /dev/null 2>&1
}

function linux_configure
{
	premake4 gmake

	if [ $? -ne 0 ]; then
		exit;
	fi

	do_make src

	cd bin
	export LD_LIBRARY_PATH=.
	cd -
}

function windows_configure
{
	premake4 vs2012

	sed -i "s/v110/v140/g" src/insolence_toy.vcxproj
	sed -i "s/Level3/TurnOffAllWarnings/g" src/insolence_toy.vcxproj

	scripts/winbuild.bat
}

function webgl_configure
{
	premake4 gmake

	do_make src

	cd bin

	ln -s insolence_toy insolence_toy.bc /dev/null 2&>1

	local webgl_command="\
emcc \
libinsolence.bc insolence_toy.bc \
--preload-file shaders --preload-file assets --memory-init-file 1 \
-s USE_SDL=2 -s USE_FREETYPE=1 -s TOTAL_MEMORY=32777216 \
-o insolence_toy.js"

	if [ "$CONFIGURATION" == "webgl-release" ]; then
		webgl_command="${webgl_command} -O3"
	fi

	$webgl_command

	if [ $? -ne 0 ]; then
		exit
	fi

	cp "../templates/insolence.html" "insolence_toy.html"
	cp "../templates/insolence.css" .
	cp "../templates/insolence.svg" .

	local IFS=''
	local i=0
	while read line; do
		i=$((i + 1))
		if [ $i -eq 1 ]; then
			echo "var project_name = \"insolence_toy\"" > "insolence.js"
			continue;
		fi
		echo -e "$line" >> "insolence.js"
	done < "../templates/insolence.js"

	if [ "$RUN_AFTER_BUILD" == "true" ]; then
		xdg-open insolence_toy.html
	fi

	cd ..
}

for arg in "$@"; do
	shift
	case "$arg" in
		"--run")           set -- "$@" "-r" ;;
		"--webgl")         set -- "$@" "-w" ;;
		"--release")       set -- "$@" "-R" ;;
		"--help")          set -- "$@" "-h" ;;
		*)                 set -- "$@" "$arg"
	esac
done

while getopts "hcwrRe" opt; do
	case "$opt" in
		r) #--run
			RUN_AFTER_BUILD="true"
			;;
		w) #--webgl
			if [ "$PLATFORM" == "WINDOWS" ]; then
				echo "WebGL build is currently unsupported on Windows."
				exit
			fi
			PLATFORM="WEBGL"
			;;
		R)
			CONFIGURATION="release"
			;;
		h) #--help
			echo_usage
			exit
			;;
		*)
			;;
	esac
done
shift $((OPTIND-1))

if [[ "$PLATFORM" == "WEBGL" ]]; then
	if [[ "$CONFIGURATION" == "debug" ]]; then
		CONFIGURATION="webgl-debug"
	elif [[ "$CONFIGURATION" == "release" ]]; then
		CONFIGURATION="webgl-release"
	fi
fi

case "$PLATFORM" in
	"WINDOWS")
		windows_configure
		;;
	"LINUX")
		linux_configure
		;;
	"WEBGL")
		webgl_configure
		exit
		;;
esac

if [ "$RUN_AFTER_BUILD" == "true" ]; then
	cd bin
	./insolence_toy
fi
