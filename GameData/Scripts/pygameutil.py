import threading

def _filenameToModuleName (filename):
    if filename.startswith('Scripts/'):
        filename = filename[8:]
    if filename.endswith('.py'):
        filename = filename[:-3]
    return filename

threads = []

class ScriptThread(threading.Thread):
    def __init__(self, scriptName):
        threading.Thread.__init__(self, name=scriptName)
        self.scriptName = scriptName
        self.daemon = True

    def run(self):
        __import__(self.scriptName)

def runScriptFile(filename):
    scriptName = _filenameToModuleName(filename)
    thread = ScriptThread(scriptName)
    threads.append(thread)
    thread.start()

def waitForScripts():
    print 'Waiting for scripts to complete...'
    threadsAlive = True
    while threadsAlive:
        for thread in threads:
            thread.join(1)
        threadsAlive = False
        for thread in threads:
            if thread.isAlive():
                threadsAlive = True
                print thread.name, 'is still alive.'
    print 'Scripts completed.'
