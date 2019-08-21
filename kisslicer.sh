#!/bin/bash

set -e

if [ "$1" = "" ]; then
    echo "usage: <printer>"
    exit 1
fi

(
     cd /home/crpalmer/kisslicer-configs/$1
     if [ ! -e KISSlicer ]; then
          ln -s ../bin/KISSlicer
     fi
     ./KISSlicer &
)