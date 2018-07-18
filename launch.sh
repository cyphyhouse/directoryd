#!/bin/bash

python botinfoupdate.py 1 192.168.1.19
python testmainupdate.py 1 192.168.1.19
cp TestMain.java ../src/main/java/testmain/
cp BotInfoSelector.java ../src/main/java/testmain/
mvn compile
mvn clean install
git add -A
git commit -m "update"
git push
#add lines for functions for and waiting for messagespython botinfoupdate.py 2 192.168.1.18
python testmainupdate.py 2 192.168.1.18
cp TestMain.java ../src/main/java/testmain/
cp BotInfoSelector.java ../src/main/java/testmain/
mvn compile
mvn clean install
git add -A
git commit -m "update"
git push
#add lines for functions for and waiting for messages