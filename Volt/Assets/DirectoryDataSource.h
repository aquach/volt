#pragma once

#include "Volt/Core/Core.h"
#include "Volt/Assets/DataSource.h"

namespace Volt {

/**
 *  Game data in the form of a directory.
 */
class DirectoryDataSource : public DataSource {
public:
    DirectoryDataSource (const string& sourcePath);
    virtual ~DirectoryDataSource ();

    virtual bool LoadDataItem (const string& itemPath, DataItem* item) const;

    virtual bool itemExists (const string& itemPath) const;
    void WriteToPackFile (const string& packFilename);
};

}
