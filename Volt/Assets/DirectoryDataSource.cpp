#include "DirectoryDataSource.h"
#include <fstream>

namespace Volt {

DirectoryDataSource::DirectoryDataSource (const string& sourcePath)
    : DataSource(sourcePath) {
}

DirectoryDataSource::~DirectoryDataSource () {
}

bool DirectoryDataSource::LoadDataItem (const string& itemPath,
                                        DataItem* item) {
    CHECK_NOTNULL(item);

    string path = SourcePath() + "/" + itemPath;
    ifstream file(path.c_str());
    if (!file.is_open())
        return false;

    file.seekg(0, ios::end);
    int size = file.tellg();
    file.seekg(0, ios::beg);

    char* data = new char[size];
    file.read(data, size);

    item->size = size;
    item->data = data;
    item->path = path;

    return true;
}

void DirectoryDataSource::WriteToPackFile (const string& packPath) {
    string path = SourcePath() + "/" + packPath;
    ofstream out(path.c_str());
    if (!out.is_open())
        return;

    /* TODO : Also rethink interface. Should each data source (maybe more
    than two) be able to write to the other ones and read in everything in a
    common manner? Probably not. */
}

}
