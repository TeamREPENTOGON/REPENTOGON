#!/bin/bash

echo_err()
{
    echo "$@" >&2
}

usage()
{
  cat << EOF
Script used to build Repentogon and bundle a release.
Usage: ./release.sh [-h] [-m MODE] [-c CMAKE] [-s SOURCE]

Supported arguments:

-h|--help   Display this help and exit with success.

-m|--mode   Configure the mode in which to build Repentogon. This must be one
            of Debug, Release or RelWithDebInfo. Their meaning is the same as 
            in CMAKE_BUILD_TYPE.

-c|--cmake  Configure an alternative path to CMake. By default, the CMake 
            found in the PATH will be used.

--cmake-extra-args  Configure extra arguments to pass to CMake.

-s|--source Indicate the path to the top level CMakeLists.txt of Repentogon. 
            This defaults to ..

--cygwin    Assume a Cygwin environement. Converts the path to the source of
            Repentogon from UNIX style into Windows style when invoking CMake.

-y|--yes    Do not prompt the user for confirmation before initiating the 
            build of Repentogon. Useful when running in non interactive mode.
EOF
}

OPTS=$(getopt -o "g:c:m:hs:y" -l "generator:help,cmake:,mode:,cmake-extra-args:,source:,yes,cygwin" -- "$@")

if [[ $? -ne 0 ]]
then
    echo_err "Error during getopt: $?"
    exit 1
fi

eval set -- "$OPTS"
echo "OPTS = $OPTS"
unset OPTS

CMAKE=cmake
CMAKE_EXTRA_ARGS=
MODE=Debug
SOURCE_DIR=..
GENERATOR="Visual Studio 17 2022"

while true
do
  case $1 in
    "-h"|"--help")
      usage
      exit 0
      ;;

    "-c"|"--cmake")
      CMAKE=$2
      shift 2
      ;;

    "-m"|"--mode")
      MODE=$2
      shift 2
      ;;

    "--cmake-extra-args")
      CMAKE_EXTRA_ARGS=$2
      shift 2
      ;;

    "-s"|"--source")
      SOURCE_DIR=$2
      shift 2
      ;;

    "-y"|"--yes")
      YES_GIVEN=1
      shift
      ;;

    "--cygwin")
      CYGWIN_GIVEN=1
      shift
      ;;

    "-g"|"--generator")
      GENERATOR=$2
      shift 2
      ;;

    "--")
      shift
      break
      ;;

    *)
      echo_err "Unknown option '$1'"
      usage
      exit 1
      ;;
  esac
done

SOURCE_DIR=$(realpath "$SOURCE_DIR")
if [[ -n "$CYGWIN_GIVEN" ]]
then
  SOURCE_DIR=$(cygpath -w "$SOURCE_DIR")
fi

[[ ! -x "$CMAKE" ]] && { echo_err "$CMAKE is not a valid CMake executable" && exit 1; }

[[ "$MODE" != Debug && "$MODE" != Release && "$MODE" != RelWithDebInfo ]] && { echo_err "$MODE is not a valid build mode" && exit 1; }

echo "Assuming Repentogon top level source is $SOURCE_DIR"
if [[ -z "$YES_GIVEN" ]]
then
  read -p "Continue with this ? (y/n)" confirm && [[ "$confirm" == [yY] ]] || exit 1
fi

[[ -d build ]] && rm -rf build

mkdir build
cd build
"$CMAKE" -G "$GENERATOR" -A Win32 -DCMAKE_BUILD_TYPE="$MODE" $CMAKE_EXTRA_ARGS "$SOURCE_DIR" || exit 1
"$CMAKE" --build . || exit 1

cd "$MODE"
zip -r REPENTOGON.zip resources resources-repentogon libzhl.dll zhlREPENTOGON.dll freetype.dll Lua5.4.dll
sha256sum REPENTOGON.zip | grep -o "[0-9a-f]*" > hash.txt
