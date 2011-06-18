#include "DirectoryDataSource.h"
#include <fstream>
#include "PackDataSource.h"

namespace Volt {

DirectoryDataSource::DirectoryDataSource (const string& sourcePath)
    : DataSource(sourcePath) {
    LOG(INFO) << "Loading directory data...";
}

DirectoryDataSource::~DirectoryDataSource () {
}

bool DirectoryDataSource::LoadDataItem (const string& itemPath,
                                        DataItem* item) const {
    CHECK_NOTNULL(item);
    LOG(INFO) << "Loading " << itemPath << "...";

    string path = sourcePath() + "/" + itemPath;
    ifstream file(path.c_str(), ifstream::binary);
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

void DirectoryDataSource::WriteToPackFile (const string& packFilename) {
    vector<string> files;
    GetAllFilesInDirectory(sourcePath(), &files);
    PackDataSource::BuildPackFile(files, sourcePath(), packFilename);
}

}
