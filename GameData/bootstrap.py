import threading
import time

def mainThread():
    a = 5
    print 'awesome'
    for i in xrange(1, 10):
        time.sleep(1)
        print i
    
threading.Thread(target=mainThread).start()
