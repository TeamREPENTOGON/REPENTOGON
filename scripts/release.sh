#!/bin/bash

echo_err()
{
    echo "$@" >&2
}

usage()
{
  cat << EOF
Script used to download a signed Repentogon release and upload it to Github.
Usage: ./release.sh -d [OPTION]... SIGNPATH_REQUEST_ID
  or:  ./release.sh -u [OPTION]... LOCAL_ARTIFACT

Mode selection:
  -d, --download=TOKEN    Download a release from SignPath, compute its hash
                          using sha256, and both the hash and release are uploaded 
                          as a new release on GitHub. TOKEN is the token to use 
                          to authenticate with SignPath. The positional argument
                          SIGNPATH_REQUEST_ID needs to be set to the signing
                          request ID.

  -u, --upload            Upload a release from the filesystem. The hash is
                          computed using sha256 and is uploaded alongside the
                          file. LOCAL_ARTIFACT is the path to the file to
                          upload, the basename of LOCAL_ARTIFACT is the name of
                          the file once uploaded (e.g. /tmp/foo is uploaded as 
                          foo).

Mandatory release parameters:
  --token=TOKEN           Token to use for the GitHub authentication.
  --tag=TAG               Tag to associate with this release.
  
Optional release parameters:
  -n, --name=NAME         The name of the release.
  -b, --body=BODY         The description of the release.
  -d, --draft             Mark the release as a draft (unpublished).
  -p, --prerelease        Mark the release as a pre-release.
  -l, --latest=VALUE      Whether to mark the release as the latest or not.
                          Possible values are true, false or legacy.
EOF
}

OPTS=$(getopt -o "hd:un:b:pl:" -l "help,download:,upload,token:,tag:,name:,body:,draft,prerelease,latest:" -- $@)

RES=$?
[[ $RES -ne 0 ]] && { echo_err "Error during getopt: $RES"; exit 1; }

eval set -- "$OPTS"
echo "OPTS = $OPTS"
unset OPTS

SIGNPATH_REPENTOGON_ID=b3b61b76-b26e-4cfd-9b51-bb56d66476fa

DOWNLOAD_MODE="Download"
UPLOAD_MODE="Upload"
MODE=

SIGNPATH_TOKEN=
UPLOAD_TOKEN=
TAG=
NAME=
BODY=
DRAFT=
PRERELEASE=
LATEST=

while true
do
  case $1 in
    "-h"|"--help")
      usage
      exit 0
      ;;

    "-d"|"--download")
      MODE=$DOWNLOAD_MODE
      SIGNPATH_TOKEN=$2
      shift 2
      ;;

    "-u"|"--upload")
      MODE=$UPLOAD_MODE
      shift
      ;;

    "--token")
      UPLOAD_TOKEN=$2
      shift 2
      ;;

    "--tag")
      TAG=$2
      shift 2
      ;;

    "-n"|"--name")
      NAME=$2
      shift 2
      ;;

    "-b"|"--body")
      BODY=$2
      shift 2
      ;;

    "--draft")
      DRAFT=1
      shift
      ;;

    "-p"|"--prerelease")
      PRERELEASE=1
      shift
      ;;

    "l"|"--latest")
      LATEST=$2
      shift 2
      ;;

    "--")
      shift
      break
      ;;

    "*")
      echo_err "Unknown option $1"
      usage
      exit 1
      ;;
  esac
done

echo "Done parsing options"

[[ -z "$MODE" ]] && echo_err "One of -u or -d must be given" && exit 1
[[ -z "$UPLOAD_TOKEN" ]] && echo_err "--token must be given" && exit 1
[[ -z "$TAG" ]] && echo_err "--tag must be given" && exit 1
[[ -n "$LATEST" ]] && [ "$LATEST" != true -a "$LATEST" != false -a "$LATEST" != legacy ] && 
  echo_err "Invalid value $LATEST for -l/--latest. Expected one of 
  true, false, legacy." && exit 1

if [[ -z "$1" ]]
then
  echo "Missing positional argument"
  [[ "$MODE" == "$DOWNLOAD_MODE" ]] && 
    { echo_err "Missing signpath request ID" && usage && exit 1; } ||
    { echo_err "Missing artifact" && usage && exit 1; }
fi

which curl >/dev/null 2>&1 || { echo_err "curl not found, unable to proceed" && exit 1; }
which sha256sum > /dev/null 2>&1 || { echo_err "sha256sum not found, unable to proceed" && exit 1; }

if [[ "$MODE" == "$DOWNLOAD_MODE" ]]
then
  echo "Downloading signed artifact"
  curl -H "Authorization: Bearer $SIGNPATH_TOKEN" \
    -o REPENTOGON.zip \
    "https://app.signpath.io/API/v1/$SIGNPATH_REPENTOGON_ID/SigningRequests/$1/SignedArtifact"
  ARTIFACT=REPENTOGON.zip
else
  ARTIFACT=$1
fi

[[ -f "$ARTIFACT" ]] || { echo_err "Artifact $ARTIFACT not found" && exit 1; }

RELEASE_STR='{"tag_name": '
RELEASE_STR="${RELEASE_STR} \"$TAG\""
[[ -n "$NAME" ]] && RELEASE_STR="$RELEASE_STR, \"name\": \"$NAME\""
[[ -n "$BODY" ]] && RELEASE_STR="$RELEASE_STR, \"body\": \"$BODY\""
[[ -n "$DRAFT" ]] && RELEASE_STR="$RELEASE_STR, \"draft\": true"
[[ -n "$PRERELEASE" ]] && RELEASE_STR="$RELEASE_STR, \"prerelease\": true"
[[ -n "$LATEST" ]] && RELEASE_STR="$RELEASE_STR, \"latest\": \"$LATEST\""
RELEASE_STR="$RELEASE_STR }"

echo "Creating the release"
RELEASE=$(curl -L -X POST \
  -H "Accept: application/vnd.github+json" \
  -H "Authorization: Bearer $UPLOAD_TOKEN" \
  -H "X-GitHub-Api-Version: 2022-11-28" \
  https://api.github.com/repos/TeamREPENTOGON/REPENTOGON/releases \
  -d "$RELEASE_STR"
)

RELEASE_ID=$(echo "$RELEASE" | grep -o '"id": [0-9]*' | head -n 1 | sed -e 's/"id": *//' -e 's/\n//')

echo "Computing Sha256 sum"
sha256sum "$ARTIFACT" | awk '{ print $1 }' | sed -e 's/[\r\n]//g' > hash.txt

echo "Uploading the artifacts"
for file in "$ARTIFACT" hash.txt
do
  curl -L -X POST \
    -H "Accept: application/vnd.github+json" \
    -H "Authorization: Bearer $UPLOAD_TOKEN" \
    -H "X-GitHub-Api-Version: 2022-11-28" \
    -H "Content-Type: application/octet-stream" \
    "https://uploads.github.com/repos/TeamREPENTOGON/REPENTOGON/releases/$RELEASE_ID/assets?name=$file" \
    --data-binary "@$file"
done

