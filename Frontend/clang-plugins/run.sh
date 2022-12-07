#!/bin/bash

./checker.sh

if ./checker.sh | grep -q 'Error'
then
    echo 'No optis'
else
    echo 'Optis'
fi