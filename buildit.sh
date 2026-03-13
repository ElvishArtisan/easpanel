#!/bin/sh

./link_common.sh easp link
./link_common.sh tests link

cmake -B build -S .
make -C build

#./link_common.sh easp unlink
#./link_common.sh tests unlink
