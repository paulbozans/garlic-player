######################################################################
# Automatically generated by qmake (3.1) Sun Mar 5 04:07:51 2017
######################################################################

TEMPLATE = lib
TARGET = zlib
INCLUDEPATH += .
CONFIG += staticlib c++11
DESTDIR = ../../lib/

# Input
HEADERS += crc32.h \
           deflate.h \
           gzguts.h \
           inffast.h \
           inffixed.h \
           inflate.h \
           inftrees.h \
           trees.h \
           zconf.h \
           zlib.h \
           zutil.h
SOURCES += adler32.c \
           compress.c \
           crc32.c \
           deflate.c \
           gzclose.c \
           gzlib.c \
           gzread.c \
           gzwrite.c \
           infback.c \
           inffast.c \
           inflate.c \
           inftrees.c \
           trees.c \
           uncompr.c \
           zutil.c
