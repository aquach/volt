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
    ~DataItem () {
        delete[] data;
        data = NULL;
    }

    int size;
    char* data;
    string path;
    string fullpath;

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

    virtual bool LoadDataItem (const string& itemPath,
                               DataItem* item) const = 0;

    string sourcePath () const { return m_sourcePath; }

private:
    string m_sourcePath;

    DISALLOW_COPY_AND_ASSIGN(DataSource);
};

}
