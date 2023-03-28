#!/bin/bash

code="$PWD"
opts=-g
cd build/ > /dev/null
g++ $opts $code/abc -o kopernikus
cd $code > /dev/null
