#!/bin/bash
for i in `ls testgen`
do
  testgen/$i
done
