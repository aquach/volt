#pragma once

#include "Core/Core.h"
#include "DataSource.h"

namespace Volt {

/**
 *  Game data in the form of a single packed file.
 *  Format:
 *  4-byte header code (0xb1d0000f)
 *  4-byte hash of entire rest of game data.
 *  4-byte number of data items in the pack.
 *
 *  For each data item:
 *  4-byte pointer into game data pack to beginning of data of data item.
 *  4-byte length of path string
 *  Variable length path string
 *  Padding to 4-byte boundary
 *
 *  Data for each data item.
 */
class PackDataSource : public DataSource {
public:
    PackDataSource (const string& sourcePath);
    virtual ~PackDataSource ();

    virtual bool LoadDataItem (const string& itemPath, DataItem* item);

    void WriteToDirectory (const string& dirPath);
};

}
