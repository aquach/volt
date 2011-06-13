#pragma once

#include "Core/Core.h"
#include "Asset.h"

namespace Volt {

class DataItem;

/* A texture file. */
class TextureAsset : public Asset {
public:
    enum FilterType {
        FILTER_NONE,
        FILTER_LINEAR
    };

    TextureAsset () { }
    ~TextureAsset ();

    bool Load (const DataItem& item, FilterType filterType, bool repeatX,
               bool repeatY);

    int glID() const { return m_texId; }
    int width () const { return m_width; }
    int height () const { return m_height; }

    bool repeatsX () const { return m_repeatX; }
    bool repeatsY () const { return m_repeatY; }
    FilterType filterType () const { return m_filterType; }

    virtual void Reload ();
    virtual void Unload ();

private:
    int m_texId;
    int m_width;
    int m_height;

    FilterType m_filterType;
    bool m_repeatX;
    bool m_repeatY;
};

}
