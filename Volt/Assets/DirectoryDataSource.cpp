#include "DirectoryDataSource.h"
#include <fstream>

namespace Volt {

DirectoryDataSource::DirectoryDataSource (const string& sourcePath)
    : DataSource(sourcePath) {
}

DirectoryDataSource::~DirectoryDataSource () {
}

bool DirectoryDataSource::LoadDataItem (const string& itemPath,
                                        DataItem* item) const {
    CHECK_NOTNULL(item);

    string path = sourcePath() + "/" + itemPath;
    ifstream file(path.c_str());
    if (!file.is_open()) {
        LOG(FATAL) << "Path doesn't exist: " << path;
        return false;
    }

    file.seekg(0, ios::end);
    int size = file.tellg();
    file.seekg(0, ios::beg);

    char* data = new char[size];
    file.read(data, size);

    item->size = size;
    item->data = data;
    item->fullpath = path;
    item->path = itemPath;

    return true;
}

void DirectoryDataSource::WriteToPackFile (const string& packPath) {
    string path = sourcePath() + "/" + packPath;
    ofstream out(path.c_str());
    if (!out.is_open())
        return;

    /* TODO : Also rethink interface. Should each data source (maybe more
    than two) be able to write to the other ones and read in everything in a
    common manner? Probably not. */
}

}
