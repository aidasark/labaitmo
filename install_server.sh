#!/bin/bash

# https://developer.toradex.com/knowledge-base/how-to-autorun-application-at-the-start-up-in-linux#etcprofile
# https://askubuntu.com/questions/191709/how-to-run-a-program-as-a-service-silent

source env_vars.sh

echo cd $INSTDIR/server/ > server.sh
echo './server &' >> server.sh
sudo cp server.sh $SCRIPTDIR

make

mkdir $INSTDIR
cp -r server $INSTDIR
