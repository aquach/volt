#pragma once

#include "Core/Core.h"
#include "Assets/AssetManager.h"
#include "Game/Entity.h"

namespace Volt {

class Label : public Entity {
public:
	Label (FontAssetRef font, float x, float y)
		: m_font(font),
		  m_x(x),
		  m_y(y),
		  m_centered(false),
          m_color(Color::white) {
	}

	bool centered () const { return m_centered; }
	void SetCentered (bool centered) { m_centered = centered; }

	string text () const { return m_text; }
	void SetText (string text) { m_text = text; }

	Color color () const { return m_color; }
	void SetColor (Color c) { m_color = c; }

	virtual void Update () {}
	virtual void Render ();

private:
	FontAssetRef m_font;
	Color m_color;
	string m_text;
	float m_x;
	float m_y;
	bool m_centered;
};

}
