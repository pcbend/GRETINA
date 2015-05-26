#!/bin/bash
#Source me!!!

# this should be sourced, not executed, to work properly
if [[ "$_" == $0 ]]; then
    echo "Try sourcing me!";
    exit 0;
fi

SOURCEDFILE=`readlink -f $BASH_SOURCE`

export GEBSYS=`dirname $SOURCEDFILE`
echo "GEBSYS is now $GEBSYS"

if [ ! -d $ROOTSYS ]; then
    echo "ROOTSYS=$ROOTSYS does not point to a directory. Please rectify."
    return 0;
fi

export PATH=$GEBSYS/bin:$PATH
echo "Added $GEBSYS/bin to PATH"

export LD_LIBRARY_PATH=$GEBSYS/libraries:$LD_LIBRARY_PATH
echo "Added $GEBSYS/libraries to PATH."

echo
echo
echo "To avoid running this script every session add the following to your ~/.bashrc file"
echo "export GEBSYS=$GEBSYS"
echo "export PATH=\$GEBSYS/bin:\$PATH" 
echo "export LD_LIBRARY_PATH=\$GEBSYS/libraries:\$LD_LIBRARY_PATH"


# clean up variables
unset SOURCEDFILE
