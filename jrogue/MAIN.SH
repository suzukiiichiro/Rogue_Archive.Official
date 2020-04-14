#!/bin/bash

cd src ;
javac *.java ;
jar -cvf Rogue.jar * ;
cp Rogue.jar ../;
cd ../;
appletviewer index.html ;
