#include "Game/Core/Controllers/HomingController.h"
#include "Game/Game/Entity.h"

void HomingController::Update () {
    m_projectileController.SetTargetPos(m_target->position());
    m_projectileController.Update();
}
