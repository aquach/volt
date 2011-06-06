#pragma once

#include "Core/Core.h"

namespace Volt {

class Asset;

/**
 *  An item in a data source.
 */
struct DataItem {
public:
    DataItem () : size(0), data(NULL) { }

    int size;
    char* data;
    string path;

    void Unload () {
        delete data;
        data = NULL;
    }

private:
    DISALLOW_COPY_AND_ASSIGN(DataItem);
};

/**
 *  A data source for game data.
 */
class DataSource {
public:
    DataSource (const string& sourcePath) : m_sourcePath(sourcePath) { }
    virtual ~DataSource () { }

    virtual bool LoadDataItem (const string& itemPath, DataItem* item) = 0;

    string SourcePath () { return m_sourcePath; }

private:
    string m_sourcePath;

    DISALLOW_COPY_AND_ASSIGN(DataSource);
};

}
