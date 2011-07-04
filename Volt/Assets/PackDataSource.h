#pragma once

#include "Volt/Core/Core.h"
#include "Volt/Assets/DataSource.h"

namespace Volt {

const uint PACK_CODE = 0x000fb1d0;

/**
 *  Game data in the form of a single packed file.
 *  Format:
 *  4-byte header code (0xb1d0000f)
 *  4-byte hash of entire rest of game data.
 *  4-byte number of data items in the pack.
 *
 *  For each data item:
 *  4-byte pointer into game data pack to beginning of data of data item.
 *  4-byte length of file.
 *  4-byte length of path string
 *  Variable length path string
 *
 *  Data for each data item.
 */
class PackDataSource : public DataSource {
public:
    PackDataSource (const string& sourcePath);
    virtual ~PackDataSource ();

    virtual bool LoadDataItem (const string& itemPath, DataItem* item) const;

    void WriteToDirectory (const string& dirPath);

    /** Builds a pack file out of a list of filenames (relative to the source
     *  path, an absolute path to where the filenames are stored). */
    static void BuildPackFile (const vector<string>& filenames,
                               const string& sourcePath,
                               const string& packFilename);

private:
    struct PackItem {
        int offset;
        int size;
    };

    map<string, PackItem> m_files;
    char* m_data;
    int m_size;
};

}
