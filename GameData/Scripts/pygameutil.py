import threading

def _filenameToModuleName (filename):
    if filename.startswith('Scripts/'):
        filename = filename[8:]
    if filename.endswith('.py'):
        filename = filename[:-3]
    return filename

def runScriptFileThread(scriptName):
    __import__(scriptName)

def runScriptFile(filename):
    scriptName = _filenameToModuleName(filename)
    threading.Thread(target=runScriptFileThread, args=(scriptName,)).start()
