import os
import sys

path = os.path.dirname(os.path.abspath(__file__))
path = os.path.dirname(os.path.dirname(path))

sys.path.insert(0, os.path.join(path, 'Build'))
sys.path.insert(0, os.path.join(path, 'Game/Python'))
sys.path.insert(0, os.path.join(path, 'Volt/Python'))

import pygame
import pyvolt
import threading
import time

def mainThread():
    a = 5
    print 'awesome'
    for i in xrange(1, 10):
        time.sleep(1)
        print i

for i in xrange(4):
    threading.Thread(target=mainThread).start()
