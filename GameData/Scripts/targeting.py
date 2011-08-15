import math

def linearTarget(targetPos, targetVel, projectilePos, projectileSpeed):
    """Calculates how to hit a target with linear prediction.

    Predicts the time and location where a projectile should be fired to hit
    a target at at particular position moving with a particular velocity.
    Assumes that the target will move with the same velocity as they are
    currently moving. Returns a float t which is the time of collision and a
    Vector2 which is the position of collision, or None if it it is
    impossible."""

    S = projectileSpeed * projectileSpeed
    x = projectilePos.x - targetPos.x
    y = projectilePos.y - targetPos.y

    vx = targetVel.x
    vy = targetVel.y

    det = (S * x * x + S * y * y - vx * vx * y * y + 2 * vx * vy * x * y -
           vy * vy * x * x)
    if det < 0:
        return None

    denom = S - vx * vx - vy * vy
    side = -vx * x - vy * y

    det = math.sqrt(det)
    t1 = (det + side) / denom
    t2 = (-det + side) / denom

    t = t1 if t1 > 0 else t2
    return t, targetPos + targetVel * t
