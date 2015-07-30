#!/bin/bash
# Immutable Project-Local Toolchain Setup
# Inspired by Python's virtualenv and npm
# See http://wp.me/pZZvH-an for more
# Run with `source ./armenv.sh`

# TODOs:
# Add upgrade option
# Cleaner toolchain versioning
# Detect different toolchain versions and warn if installing new?

# Toolchain base string
# Set this to change toolchain versions (though perhaps this is not the most intuitive method)
TOOLCHAIN_BASE="gcc-arm-none-eabi-4_9-2015q1-20150306"

# Parse useful information from the toolchain string

if ! [[ $TOOLCHAIN_BASE =~ gcc-arm-none-eabi-([0-9])_([0-9])-([0-9]{4})q([0-4])-([0-9]{8}) ]]; then
	echo "Invalid base toolchain string"
	return
fi

TOOLCHAIN_VERSION_MAJOR=${BASH_REMATCH[1]}
TOOLCHAIN_VERSION_MINOR=${BASH_REMATCH[2]}
TOOLCHAIN_YEAR=${BASH_REMATCH[3]}
TOOLCHAIN_QUARTER=${BASH_REMATCH[4]}
TOOLCHAIN_DATE=${BASH_REMATCH[5]}

# Toolchain base URL
# example: https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q1-update/+download/gcc-arm-none-eabi-4_9-2015q1-20150306-win32.zip
TOOLCHAIN_ADDR="https://launchpad.net/gcc-arm-embedded/${TOOLCHAIN_VERSION_MAJOR}.${TOOLCHAIN_VERSION_MINOR}/${TOOLCHAIN_VERSION_MAJOR}.${TOOLCHAIN_VERSION_MINOR}-${TOOLCHAIN_YEAR}-q${TOOLCHAIN_QUARTER}-update/+download/"

# Toolchain folder name
# This must match that of the extracted archive
TOOLCHAIN_LOC=gcc-arm-none-eabi-${TOOLCHAIN_VERSION_MAJOR}_${TOOLCHAIN_VERSION_MINOR}-${TOOLCHAIN_YEAR}q${TOOLCHAIN_QUARTER}

WORKDIR=`pwd`
TOOLDIR="$WORKDIR/.env"

# Check if tools exist
TOOLTEST="$TOOLDIR/$TOOLCHAIN_LOC/bin/arm-none-eabi-gcc"

res=$?
if [ -f ${TOOLTEST} ] 
then
	export PATH=$TOOLDIR/$TOOLCHAIN_LOC/bin:$PATH
	echo "Toolchain loaded (already installed)"
	return
fi

# Create tool directory
mkdir .env

# Determine toolchain file names

echo "Determining required toolchain"
if [ "$(uname)" == "Darwin" ]; then
    # Do something under Mac OS X platform   
    OS="OSX"
    TOOLCHAIN_FILE="$TOOLCHAIN_BASE-mac.tar.bz2"
    TOOLCHAIN_DL="$TOOLCHAIN_ADDR$TOOLCHAIN_FILE"

elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    # Do something under Linux platform
    OS="LINUX"
    TOOLCHAIN_FILE="$TOOLCHAIN_BASE-linux.tar.bz2"
    TOOLCHAIN_DL="$TOOLCHAIN_ADDR$TOOLCHAIN_FILE"

elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
    # Do something under Windows NT platform
    OS="WIN"
    TOOLCHAIN_FILE="$TOOLCHAIN_BASE-win32.zip"
    TOOLCHAIN_DL="$TOOLCHAIN_ADDR$TOOLCHAIN_FILE"
fi

# Check if toolchain is cached

CACHED="$HOME/.env/$TOOLCHAIN_FILE"
if [ -f $CACHED ]; then
    echo "Toolchain found at: $CACHED"
else
    echo "Cached toolchain not found, downloading"
    wget $TOOLCHAIN_DL -P $HOME/.env
fi
cp $CACHED $TOOLDIR

#Extract toolchain

echo "Extracting toolchain"
if [ "$OS" == "OSX" ]; then
	tar -xf $TOOLDIR/$TOOLCHAIN_FILE -C $TOOLDIR
elif [ "$OS" == "LINUX" ]; then
	tar -xf $TOOLDIR/$TOOLCHAIN_FILE -C $TOOLDIR
elif [ "$OS" == "WIN" ]; then
	unzip $TOOLDIR/$TOOLCHAIN_FILE -d $TOOLDIR
fi

# Setup environment

if [ "$OS" == "OSX" ]; then
    ln -s $TOOLDIR/$TOOLCHAIN_FILE $TOOLDIR/gcc-arm-none-eabo
elif [ "$OS" == "LINUX" ]; then
    ln -s $TOOLDIR/$TOOLCHAIN_FILE $TOOLDIR/gcc-arm-none-eabo
elif [ "$OS" == "WIN" ]; then
    echo "Unimplemented"
    return -1;
fi

export PATH=$TOOLDIR/$TOOLCHAIN_LOC/bin:$PATH

echo "Toolchain loaded"
