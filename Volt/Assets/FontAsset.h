#pragma once

#include "Volt/Core/Core.h"
#include "Volt/Assets/Asset.h"

namespace Volt {

class DataItem;

class FontAsset : public Asset {
public:
    FontAsset (int textureWidth, int textureHeight);
    ~FontAsset ();

    bool Load (const DataItem& item, float size);

    virtual void Reload ();
    virtual void Unload ();

    /** Gets the information for where to render the given character. x and y
     *  are the desired location for the texture, and are updated to be the new
     *  position after rendering the current quad with the data returned in the
     *  BBoxes. */
    void GetGlyphData (char c, float* x, float* y,
                       BBox* verts, BBox* texCoords) const;

    float GetTextWidth (const string &text);
    float GetTextHeight (const string &text);

    int glID () const { return m_texID; }
    float size () const { return m_size; }

    virtual string assetKey () const {
        return BuildKey(path(), m_size);
    }

    // Needed by AssetManager when looking up assets.
    static string BuildKey (const string& path, float size) {
        char buffer[512];
        sprintf(buffer, "%s-%.3f", path.c_str(), size);
        return buffer;
    }


private:
    void* m_fontCData;
    int m_texID;
    float m_size;
    int m_textureWidth;
    int m_textureHeight;
};

}
