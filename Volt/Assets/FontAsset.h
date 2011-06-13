#pragma once

#include "Core/Core.h"
#include "Asset.h"

namespace Volt
{

class DataItem;

class FontAsset : public Asset {
public:
    FontAsset (int textureWidth, int textureHeight);
    ~FontAsset ();

    // TODO: What happens if you load same filename with two sizes?
    bool Load (const DataItem& item, float size);

    void Reload ();
    void Unload ();

    /** Gets the information for where to render the given character. x and y
     *  are the desired location for the texture, and are updated to be the new
     *  position after rendering the current quad with the data returned in the
     *  BBoxes. */
    void GetGlyphData (char c, float* x, float* y,
		       BBox* verts, BBox* texCoords) const;

    float GetTextWidth (const string &text);
    float GetTextHeight (const string &text);

    int glID () const { return m_texID; }

private:
    void* m_fontCData;
    int m_texID;
    float m_size;
    int m_textureWidth;
    int m_textureHeight;
};

}
