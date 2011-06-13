#pragma once

#include "Core/Core.h"
#include "Assets/AssetManager.h"

namespace Volt {

class Window;

class Graphics {

public:
	enum BlendType {
		BLEND_NONE = 0,
		BLEND_ALPHA,
		BLEND_ADDITIVE,
		BLEND_MULTIPLY,
	};

	static void Set2D (int virtualWidth, int virtualHeight);
	static const Vector2& GetResolutionScale ();

	static void BindTexture (TextureAssetRef textureAsset);
	static void BindFont (FontAssetRef fontAsset);

	static void Translate (const Vector2& pos);
	static void Scale (const Vector2& pos);

	static void RenderQuad (float width,
						    float height,
						    const Vector2& textureOffset = Vector2::zero,
						    const Vector2 &textureScale = Vector2::one,
						    const Vector2 &position = Vector2::zero);

	static void RenderQuadCustom (const Vector2& ul,
								  const Vector2& ur,
								  const Vector2& lr,
								  const Vector2& ll,
								  const Vector2& textureOffset,
								  const Vector2& textureScale);

	static void RenderText (FontAssetRef font, const string& text,
							float x, float y);
	static void RenderTextCentered (FontAssetRef font, const string& text,
								    float x, float y);

	static void RenderLineRect (float x, float y, float w, float h);
	static void RenderLineRect (const BBox& box);
	static void RenderLine (const Vector2& pos1, const Vector2& pos2);

	static void SetBlend (BlendType blend);

	static void SetBackgroundColor (const Color& color);
	static Color GetBackgroundColor ();
	static void EnableBackgroundReset (bool bgReset = true);

	static Vector2 GetMatrixPosition ();
	static Vector2 GetScreenCenter ();
	static int GetVirtualWidth ();
	static int GetVirtualHeight ();

	static void ResolutionMatrix ();
	static void IdentityMatrix ();
	static void DefaultMatrix ();

	Graphics (Window* window);
	void Init ();

	void ShowBuffer();

private:
	static Graphics* instance;

	Vector2 resolutionScale;
	Vector2 screenCenter;
	int virtualWidth, virtualHeight;
	int lastBoundTextureID;
	BlendType currentBlend;
	Window* m_window;
};

}
