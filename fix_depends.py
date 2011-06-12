#!/usr/bin/python

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
