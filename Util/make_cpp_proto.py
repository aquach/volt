#!/usr/bin/python

import re
import sys

def main():
    headers = sys.stdin.read().splitlines()
    className = None
    for header in headers:
        header = header.strip()
        if not header:
            continue

        if header.startswith('virtual '):
            header = header[8:]
        match = re.match(r'(\w+) \(\);', header)
        header = header[:-1]
        isSpecial = False
        if match:
            className = match.group(1)
            isSpecial = True
        if header.find('~') != -1:
            isSpecial = True

        if isSpecial:
            print '%s::%s {\n}\n' % (className, header)
        else:
            chunks = header.split(None, 1)
            print '%s %s::%s {\n}\n' % (chunks[0], className, chunks[1])



if __name__ == '__main__':
    main()
