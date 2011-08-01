import threading

def _filenameToModuleName (filename):
    if filename.startswith('Scripts/'):
        filename = filename[8:]
    if filename.endswith('.py'):
        filename = filename[:-3]
    return filename

class ScriptThread(threading.Thread):
    def __init__(self, scriptName):
        threading.Thread.__init__(self, name=scriptName)
        self.scriptName = scriptName
        self.daemon = True

    def run(self):
        __import__(self.scriptName)

def runScriptFile(filename):
    scriptName = _filenameToModuleName(filename)
    ScriptThread(scriptName).start()
