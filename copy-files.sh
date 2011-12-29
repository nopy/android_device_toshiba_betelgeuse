#!/bin/sh

BASE=../../../vendor/toshiba/betelgeuse/proprietary
rm -rf $BASE/*

for FILE in `cat proprietary-files.txt`; do
DIR=`dirname $FILE`
    if [ ! -d $BASE/$DIR ]; then
mkdir -p $BASE/$DIR
    fi
cp -f $1/system/$FILE $BASE/$FILE
done

./setup-makefiles.sh

