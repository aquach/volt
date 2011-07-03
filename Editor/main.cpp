#include <QtGui/QApplication>
#include "Editor/Editor.h"
#include "Core/Core.h"
#include "Volt/Assets/DirectoryDataSource.h"

int main(int argc, char *argv[]) {
	string exeDir = Volt::GetExecutableDirectory(argv[0]);
	LOG(INFO) << "Executable directory: " << exeDir;
    
    QApplication app(argc, argv);
    Editor editor(new Volt::DirectoryDataSource(exeDir + "../GameData"));
    editor.show();

    return app.exec();
}
