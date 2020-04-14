#!/bin/bash


javac -cp .:bin:asciiPanel.jar src/rltut/ApplicationMain.java;
echo "building..." ;
java -cp .:bin:asciiPanel.jar rltut/ApplicationMain;
exit ;
