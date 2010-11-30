#!/bin/sh
aclocal
autoheader
libtoolize -i
automake -a
autoconf
