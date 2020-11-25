#!/bin/bash

source env_vars.sh

# for demo we have two keys
# in real programm it will be one
echo '' > $INSTDIR/server/exp_keys/0
echo '' > $INSTDIR/server/exp_keys/1

rm -rf $INSTDIR/client
