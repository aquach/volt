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

struct PackFileHeader {
    unsigned int code;
    unsigned int hash;
    unsigned int numItems;
};

void PackDataSource::BuildPackFile (const vector<string>& filenames,
                                    const string& packFilename) {
    LOG(INFO) << "Building pack file " << packFilename;

    ofstream out(packFilename.c_str(), ofstream::binary);
    if (!out.is_open()) {
        LOG(ERROR) << "Could not open " << packFilename;
        return;
    }

    // Enough room for a small pack file.
    int memSize = 3 * 1024 * 1024 + sizeof(PackFileHeader)
                  + 20 * filenames.size();

    char* packFile = (char*)malloc(memSize);

    PackFileHeader* header = (PackFileHeader*)packFile;
    header->code = PACK_CODE;
    header->numItems = filenames.size();

    // Pointers into the pack file where offsets should go.
    char* filePtr = (char*)(header + 1);
    vector<int*> offsets;
    for (unsigned int i = 0; i < filenames.size(); i++) {
        // Offset.
        offsets.push_back((int*)filePtr);
        filePtr += sizeof(int);

        // String size.
        *(int*)filePtr = filenames[i].size();
        filePtr += sizeof(int);

        // String.
        memcpy(filePtr, filenames[i].c_str(), filenames[i].size());
        filePtr += filenames[i].size();

        CHECK_LE(filePtr, packFile + memSize);
    }

    // Write data.
    for (unsigned int i = 0; i < filenames.size(); i++) {
        ifstream file(filenames[i].c_str());
        if (!file.is_open()) {
            LOG(ERROR) << "Path doesn't exist: " << filenames[i];
            return;
        }

        file.seekg(0, ios::end);
        int size = file.tellg();
        file.seekg(0, ios::beg);

        if (filePtr - packFile > memSize - size) {
            memSize = (memSize + size) * 2;
            int len = filePtr - packFile;
            packFile = (char*)realloc(packFile, memSize);
            filePtr = packFile + len;
        }
        file.read(filePtr, size);

        // Write offset for corresponding header.
        *offsets[i] = filePtr - packFile;
        filePtr += size;
    }

    int packFileLength = filePtr - packFile;
    header->hash = HashData(packFile, packFileLength);

    out.write(packFile, packFileLength);
    out.close();
    free(packFile);
    LOG(INFO) << "Wrote pack file (" << packFileLength << " bytes).";
}

}
