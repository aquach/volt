#include "Game/Editor/SelectionManager.h"
#include "Game/Game/Entity.h"
#include "Game/Graphics/Graphics.h"

SelectionManager* SelectionManager::instance = NULL;

void SelectionManager::Render () {
    FOR_(set<Entity*>::iterator, i, m_selectedEntities) {
        Entity* entity = *i;
        b2AABB aabb;
        bool first = true;
        b2Fixture* f = entity->body()->GetFixtureList();
        while (f != NULL) {
            b2AABB box;
            f->GetShape()->ComputeAABB(&box, entity->body()->GetTransform());
            if (first) {
                aabb = box;
                first = false;
            } else {
                aabb.Combine(aabb, box);
            }
            f = f->GetNext();
        }

        Vector2 center = aabb.GetCenter();
        Vector2 extents = aabb.GetExtents();
        extents *= 2;
        glPushMatrix();
        glLineWidth(1.0f);
        Graphics::SetBlend(Graphics::BLEND_ALPHA);
        Graphics::Translate(center);
        Graphics::SetColor(Volt::Color::RGBA(200, 0, 0, 64));
        Graphics::RenderQuad(extents.x, extents.y);
        Graphics::SetBlend(Graphics::BLEND_NONE);
        Graphics::SetColor(Volt::Color::RGB(128, 128, 128));
        Graphics::RenderLineRect(0, 0, extents.x, extents.y);
        glPopMatrix();
    }
}
