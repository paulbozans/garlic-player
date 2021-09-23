#!/bin/bash

echo 
echo ========== apply branding
echo


# check if called from jenkins
if [ -z "$SCRIPTDIR" ]; then
        export SCRIPTDIR=$(dirname "$0")
fi


export GARLIC_APP_NAME="garlic-player"
export GARLIC_DEF_URL_NAME="SmilControl"
export GARLIC_DEF_URL="http://indexes.smil-control.com"

echo 
echo ========== write branding information to branding.h
echo 

echo "#define garlic_app_name \"$GARLIC_APP_NAME\"" > $SCRIPTDIR/src/garlic-lib/branding.h
echo "#define garlic_def_url_name \"$GARLIC_DEF_URL_NAME\"" >> $SCRIPTDIR/src/garlic-lib/branding.h
echo "#define garlic_def_url \"$GARLIC_DEF_URL\"" >> $SCRIPTDIR/src/garlic-lib/branding.h

echo ========== branding.h content
cat $SCRIPTDIR/src/garlic-lib/branding.h

