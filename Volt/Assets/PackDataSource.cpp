#include "Volt/Assets/PackDataSource.h"
#include <fstream>
#include "Volt/Assets/Hash.h"

namespace Volt {

struct PackFileHeader {
    uint code;
    uint hash;
    uint numItems;
};

PackDataSource::PackDataSource (const string& sourcePath)
    : DataSource(sourcePath),
      m_data(NULL),
      m_size(0) {

    LOG(INFO) << "Loading pack file...";

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

    uint fileHash = header->hash;
    header->hash = 0;
    uint hash = HashData(m_data, m_size);
    if (fileHash != hash) {
        LOG(ERROR) << "Invalid pack file (hash invalid).";
        delete[] m_data;
        return;
    }

    char* filePtr = (char*)(header + 1);
    for (uint i = 0; i < header->numItems; i++) {
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

bool PackDataSource::itemExists (const string& itemPath) const {
    map<string, PackItem>::const_iterator i = m_files.find(itemPath);
    return i != m_files.end();
}

bool PackDataSource::LoadDataItem (const string& itemPath,
                                   DataItem* item) const {
    CHECK_NOTNULL(item);
    LOG(INFO) << "Loading " << itemPath << "...";

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

    vector<char> packFile;
    packFile.reserve(memSize);

    packFile.resize(packFile.size() + sizeof(PackFileHeader));
    PackFileHeader* header = (PackFileHeader*)&packFile.front();
    header->code = PACK_CODE;
    header->numItems = filenames.size();

    // Pointers into the pack file where offsets should go.
    char* filePtr = (char*)(header + 1);
    vector<int> offsets;
    vector<int> sizes;
    for (uint i = 0; i < filenames.size(); i++) {
        packFile.resize(packFile.size() + sizeof(int) * 3);

        // Offset.
        offsets.push_back(filePtr - &packFile.front());
        filePtr += sizeof(int);

        // File size.
        sizes.push_back(filePtr - &packFile.front());
        filePtr += sizeof(int);

        // String size.
        *(int*)filePtr = filenames[i].size();
        filePtr += sizeof(int);

        // String.
        packFile.resize(packFile.size() + filenames[i].size());
        memcpy(filePtr, filenames[i].c_str(), filenames[i].size());
        filePtr += filenames[i].size();

        CHECK_LE(filePtr - &packFile.front(), packFile.size());
    }

    // Write data.
    for (uint i = 0; i < filenames.size(); i++) {
        string fullPath = sourcePath + "/" + filenames[i];
        ifstream file(fullPath.c_str());
        if (!file.is_open()) {
            LOG(ERROR) << "Path doesn't exist: " << fullPath;
            return;
        }

        file.seekg(0, ios::end);
        int size = file.tellg();
        file.seekg(0, ios::beg);

        *(int*)&packFile[sizes[i]] = size;

        int index = filePtr - &packFile.front();
        packFile.resize(packFile.size() + size);
        filePtr = &packFile.front() + index;
        CHECK_LE(filePtr - &packFile.front() + size, packFile.size());
        file.read(filePtr, size);

        // Write offset for corresponding header.
        *(int*)&packFile[offsets[i]] = filePtr - &packFile.front();
        filePtr += size;
        file.close();
    }

    header = (PackFileHeader*)&packFile.front();
    header->hash = HashData(&packFile.front(), packFile.size());

    out.write(&packFile.front(), packFile.size());
    out.close();
    LOG(INFO) << "Wrote pack file (" << packFile.size() << " bytes).";
}

}
