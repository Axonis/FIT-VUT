#!/bin/bash
wget http://www.stud.fit.vutbr.cz/~xurban66/img.zip
wget http://www.stud.fit.vutbr.cz/~xurban66/game_from_22-10-33.dat
unzip img.zip
mv img lib
mv game_from_22-10-33.dat examples
rm img.zip
