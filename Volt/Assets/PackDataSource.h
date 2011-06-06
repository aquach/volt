#pragma once

#include "Core/Core.h"
#include "DataSource.h"

namespace Volt {

/**
 *  Game data in the form of a single packed file.
 */
class PackDataSource : public DataSource {
public:
    PackDataSource (const string& sourcePath);
    virtual ~PackDataSource ();

    virtual bool LoadDataItem (const string& itemPath, DataItem* item);

    void WriteToDirectory (const string& dirPath);
};

}
