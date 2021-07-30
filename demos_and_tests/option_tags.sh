#!/bin/bash

echo "This command print \"def_test\" function:"
echo "$ silentbob --tags def_test"
echo 

. ./head.sh
cd $WORKDIR
silentbob -c
silentbob --tags def_test
