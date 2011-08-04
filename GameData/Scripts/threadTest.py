from pygame import *

import threading
import time

@background
def thing():
    while True:
        start = time.time()
        time.sleep(0.001)
        print 'slept for', time.time() - start
thing()
