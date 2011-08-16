import threading
import pyvoltbootstrap

"""Provides functionality for the game scripting engine to run scripts in
separate threads."""

def _filenameToModuleName (filename):
    """Converts a game data path to a module name."""
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
    """Runs the given script in a separate thread."""
    thread = ScriptThread(code, origin)
    threads.append(thread)
    thread.start()

def runScriptFile(filename, forceReload = False):
    """Runs the given script filename in a separate thread."""
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
    """Waits for all running scripts to complete."""
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
