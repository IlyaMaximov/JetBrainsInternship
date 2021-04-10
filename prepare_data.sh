#!/bin/bash

rm data/words.txt 2>/dev/null
mkdir data 2>/dev/null

wget -O data/words.txt https://raw.githubusercontent.com/dwyl/english-words/master/words.txt
