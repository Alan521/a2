#!/bin/bash

counter=1
while [ $counter -le 5 ]
do
  ./client 127.0.0.1:2300&
  ((counter++))
done

