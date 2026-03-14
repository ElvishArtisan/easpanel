#!/bin/sh

#
# Check the configuration
#
if test -z $FLATPAK_REPO_COLLECTION_ID ; then
    echo >&2 "No FLATPAK_REPO_COLLECTION_ID value defined, exiting..."
    exit 1
fi
if test -z $FLATPAK_REPO_PATH ; then
    echo >&2 "No FLATPAK_REPO_PATH value defined, exiting..."
    exit 1
fi
if test -z $FLATPAK_SIGNING_KEY ; then
    echo >&2 "No FLATPAK_SIGNING_KEY value defined, exiting..."
    exit 1
fi

#
# Do the export
#
flatpak build-export \
	--collection-id=$FLATPAK_REPO_COLLECTION_ID \
	--gpg-sign=$FLATPAK_SIGNING_KEY \
	$FLATPAK_REPO_PATH \
	flatpak

#
# Post it
#
rsync -rptgoDv --copy-links --delete $FLATPAK_REPO_PATH/ $FLATPAK_POST_RSYNC
