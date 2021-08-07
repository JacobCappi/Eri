#!/bin/bash
# Build script for rebuilding everything
set echo on

echo "Building Eri..."


pushd engine
source build.sh
popd

ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
echo "Error:"$ERRORLEVEL && exit
fi

pushd testbed
source build.sh
popd
ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
echo "Error:"$ERRORLEVEL && exit
fi

echo "Eri built successfully" 
