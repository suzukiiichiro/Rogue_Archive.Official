#!/bin/bash

tar -zxvf rogue_s.tar.gz;
cp rogue1_3linux.patch  rogue_s/ ;
cd rogue_s ;
patch<rogue1_3linux.patch ;
make -f Makefile.redhat ;



