#include "Transform.h"
#include "Math.h"

namespace Monocle
{
	Transform::Transform()
		: position(Vector2::zero), rotation(0.0f), scale(Vector2::one)
	{
	}

	Transform::Transform(const Transform &transform)
		: position(transform.position), rotation(transform.rotation), scale(transform.scale)
	{
	}

	void Transform::LerpTransform(Transform *prev, Transform *next, float percent)
	{
		position = next->position*percent + prev->position*(1.0f - percent);
		rotation = next->rotation*percent + prev->rotation*(1.0f - percent);
		scale = next->scale*percent + prev->scale*(1.0f-percent);
	}

    Vector2 Transform::GetDirectionVector()
    {
        return Vector2(sin(rotation*pi / 180.0),cos(rotation*pi / 180.0)*-1.0);
    }
}
