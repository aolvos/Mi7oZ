#!/bin/sh
xxd -i menu.png res.h
xxd -i font.png >> res.h
xxd -i logo.png >> res.h
xxd -i mus.xm >> res.h
cp res.h ../
rm res.h