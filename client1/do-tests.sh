#!/bin/bash

for block in 1 2 3 4 5
do
  for k in 1 2 3 4 6 7 8 9
  do
    ./client localhost 1111 "unbelievable! It works! (client1)" <commands.txt &
  done
done
