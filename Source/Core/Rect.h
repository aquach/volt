#pragma once

#include "Vector2.h"


namespace Volt {
	class Rect {
	public:
		Vector2 min, max;

		Rect ();
		Rect (const Vector2& min, const Vector2& max);
		Rect (const Rect& other);

		void UnionWith (const Rect& other);
	};
}
