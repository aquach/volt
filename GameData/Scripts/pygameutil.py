import threading
import pyvoltbootstrap

def _filenameToModuleName (filename):
    if filename.startswith('Scripts/'):
        filename = filename[8:]
    if filename.endswith('.py'):
        filename = filename[:-3]
    return filename

threads = []

class ScriptThread(threading.Thread):
    def __init__(self, code, scriptName=None):
        threading.Thread.__init__(self, name=scriptName)
        self.code = code
        self.daemon = True

    def run(self):
        exec self.code in {}

def runScript(code, origin=None):
    thread = ScriptThread(code, origin)
    threads.append(thread)
    thread.start()

def runScriptFile(filename, forceReload = False):
    path = 'Scripts/' + filename
    if forceReload:
        reloaded = pyvoltbootstrap.reloadScript(path)
        if not reloaded:
            print 'Warning: failed to reload script file', path
            return
    code = pyvoltbootstrap.getCode(path)
    if not code:
        print 'Warning: Could not execute script file', path
        return
    runScript(code, filename)

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
