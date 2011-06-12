#pragma once

#include "Core/Core.h"
#include "Asset.h"

namespace Volt
{

class DataItem;

class FontAsset : public Asset {
public:
    FontAsset ();
    ~FontAsset ();

    // TODO: What happens if you load same filename with two sizes?
    bool Load (const DataItem& item, float size,
	       int textureWidth = -1, int textureHeight = -1);

    void Reload ();
    void Unload ();

    void GetGlyphData (char c, float* x, float* y,
		       BBox& verts, BBox& texCoords) const;

    float GetTextWidth (const string &text);
    float GetTextHeight (const string &text);

private:
    void* m_fontCData;
    int m_texID;
    float m_size;
    int m_textureWidth;
    int m_textureHeight;
};

}
