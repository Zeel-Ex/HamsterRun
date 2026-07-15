#!/bin/bash
PROJDIR="$1"
HASH=$(git -C "$PROJDIR" rev-parse --short HEAD)
INIFILE="$PROJDIR/Config/DefaultGame.ini"

sed -i.bak "s/^ProjectVersion=.*/ProjectVersion=0.1.0-$HASH/" "$INIFILE"
rm -f "$INIFILE.bak"
