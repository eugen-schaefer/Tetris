#!/bin/sh

# changes to current working directory
cd "$(dirname "$0")"

echo Starting unit tests ... 

# create build directory on the project's top level if not existent
directory="./../build"
if [ ! -d $directory ]; then
	echo "Creating build directory"
    mkdir $directory
fi

# change into the build directory and make
cd $directory
echo Build all ... 
cmake ..
make
echo
echo =======================================
echo Run GameBackgroundTest ... 
echo =======================================
echo
./test/GameBackgroundTest

echo
echo =======================================
echo Run TetrominoTest ... 
echo =======================================
echo
./test/TetrominoTest


