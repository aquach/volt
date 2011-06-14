#include "PackDataSource.h"
#include <fstream>
#include "Hash.h"

namespace Volt {

struct PackFileHeader {
    unsigned int code;
    unsigned int hash;
    unsigned int numItems;
};

PackDataSource::PackDataSource (const string& sourcePath)
    : DataSource(sourcePath),
      m_data(NULL),
      m_size(0) {

    ifstream file(sourcePath.c_str(), ifstream::binary);
    if (!file.is_open()) {
        LOG(ERROR) << "Could not open " << sourcePath;
        return;
    }

    file.seekg(0, ios::end);
    m_size = file.tellg();
    file.seekg(0, ios::beg);

    m_data = new char[m_size];
    file.read(m_data, m_size);
    file.close();

    PackFileHeader* header = (PackFileHeader*)m_data;
    if (header->code != PACK_CODE) {
        LOG(ERROR) << "Invalid pack file (pack code invalid).";
        delete[] m_data;
        return;
    }

    unsigned int fileHash = header->hash;
    header->hash = 0;
    unsigned int hash = HashData(m_data, m_size);
    if (fileHash != hash) {
        LOG(ERROR) << "Invalid pack file (hash invalid).";
        delete[] m_data;
        return;
    }

    char* filePtr = (char*)(header + 1);
    for (unsigned int i = 0; i < header->numItems; i++) {
        // Ensure room to read three ints.
        if ((int)(m_size - (filePtr - m_data)) < sizeof(int) * 3) {
            LOG(ERROR) << "Invalid pack file.";
            delete[] m_data;
            return;
        }

        int offset = *(int*)filePtr;
        filePtr += sizeof(int);
        int fileSize = *(int*)filePtr;
        filePtr += sizeof(int);
        int strLen = *(int*)filePtr;
        filePtr += sizeof(int);
        char filename[512];
        memset(filename, 0, 512);
        if (strLen > 512 || m_size - (filePtr - m_data) < strLen) {
            LOG(ERROR) << "Invalid pack file.";
            delete[] m_data;
            return;
        }
        memcpy(filename, filePtr, strLen);
        filePtr += strLen;

        PackItem item = { offset, fileSize };
        m_files[filename] = item;
    }
}

PackDataSource::~PackDataSource () {
    delete[] m_data;
}

bool PackDataSource::LoadDataItem (const string& itemPath,
                                   DataItem* item) const {
    CHECK_NOTNULL(item);

    map<string, PackItem>::const_iterator i = m_files.find(itemPath);
    if (i == m_files.end()) {
        LOG(FATAL) << "Path doesn't exist: " << itemPath;
        return false;
    }

    PackItem packItem = i->second;
    CHECK_LE(packItem.offset, m_size);

    item->path = itemPath;
    item->fullpath = sourcePath() + "/" + itemPath;
    item->size = packItem.size;
    item->data = new char[packItem.size];
    memcpy(item->data, m_data + packItem.offset, packItem.size);

    return true;
}

void PackDataSource::WriteToDirectory (const string& dirPath) {
    // TODO
}

void PackDataSource::BuildPackFile (const vector<string>& filenames,
                                    const string& sourcePath,
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
    vector<int*> sizes;
    for (unsigned int i = 0; i < filenames.size(); i++) {
        // Offset.
        offsets.push_back((int*)filePtr);
        filePtr += sizeof(int);

        // File size.
        sizes.push_back((int*)filePtr);
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
        string fullPath = sourcePath + "/" + filenames[i];
        ifstream file(fullPath.c_str());
        if (!file.is_open()) {
            LOG(ERROR) << "Path doesn't exist: " << fullPath;
            return;
        }

        file.seekg(0, ios::end);
        int size = file.tellg();
        file.seekg(0, ios::beg);

        *sizes[i] = size;

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

        file.close();
    }

    int packFileLength = filePtr - packFile;
    header->hash = HashData(packFile, packFileLength);

    out.write(packFile, packFileLength);
    out.close();
    free(packFile);
    LOG(INFO) << "Wrote pack file (" << packFileLength << " bytes).";
}

}
