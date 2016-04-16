#!/bin/bash
cd ../toluaAPI/
tolua++ -L custom.lua -o toluaapi.cpp toluaapi.pkg
