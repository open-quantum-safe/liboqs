#!/bin/bash

ld -r $1 -o $1 -unexported_symbols_list $2
