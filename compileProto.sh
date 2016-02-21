#!/bin/bash

LD_LIBRARY_PATH=./usr/lib ./usr/bin/protoc --cpp_out=./ PBNetStat.proto

