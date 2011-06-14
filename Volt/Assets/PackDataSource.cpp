#include "PackDataSource.h"
#include <fstream>
#include "Hash.h"

namespace Volt {

PackDataSource::PackDataSource (const string& sourcePath)
    : DataSource(sourcePath) {
}

PackDataSource::~PackDataSource () {
}

bool PackDataSource::LoadDataItem (const string& itemPath,
                                   DataItem* item) {
    CHECK_NOTNULL(item);
    // TODO
    return false;
}

void PackDataSource::WriteToDirectory (const string& dirPath) {
    // TODO
}

}
