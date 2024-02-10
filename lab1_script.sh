#!/bin/bash
set +x
#set -- ${1:-xyz} ${@:2:$#} ; echo $*

MASK=${1}
SEARCHDIR=${2}

echo
echo
echo "Starting script..."
echo "DIR = $SEARCHDIR"
echo "OPT = $MASK"
echo
echo

for entry in "$SEARCHDIR"/*	
do
    CUROPT=$(stat -c%A --format=%A "$entry")
    CUROPT=${CUROPT:0:1}
    
    case "$MASK" in 
    *"$CUROPT"*)
         echo $entry
#         echo $(stat -c%A --format=%A "$entry")
        ;;
    esac
done

echo
echo
echo "Script finished."
echo