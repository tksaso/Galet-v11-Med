#!/bin/bash
bsub -J 'Galet[1-5]' -o "out.%J.%I" -e "err.%J.%I" ./$1
#
