#!/usr/bin/python

'''Appends the path of the filename to the beginning of the contents of the
   file. When ran on Obj/main.d which starts with "main.o," fixes it to say
   Obj/main.o.'''
import os
import sys

def main():
    if len(sys.argv) <= 1:
        return

    filename = sys.argv[1]
    path = os.path.dirname(filename)

    handle = open(filename)
    contents = handle.read()
    handle.close()

    contents = path + '/' + contents

    handle = open(filename, 'w')
    handle.write(contents)
    handle.close()

if __name__ == '__main__':
    main()
