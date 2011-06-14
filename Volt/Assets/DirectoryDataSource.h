#pragma once

#include "Core/Core.h"
#include "DataSource.h"

namespace Volt {

/**
 *  Game data in the form of a directory.
 */
class DirectoryDataSource : public DataSource {
public:
    DirectoryDataSource (const string& sourcePath);
    virtual ~DirectoryDataSource ();

    virtual bool LoadDataItem (const string& itemPath, DataItem* item) const;

    void WriteToPackFile (const string& packFilename);
};

}
