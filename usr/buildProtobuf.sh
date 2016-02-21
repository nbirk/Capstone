#!/bin/bash
INSTALL_DIR=$PWD

tar xf protobuf-2.6.1.tar.gz

pushd protobuf-2.6.1

./configure --prefix=$INSTALL_DIR
make
make install

popd

