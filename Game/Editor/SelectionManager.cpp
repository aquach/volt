#include "Game/Editor/SelectionManager.h"
#include "Game/Game/Entity.h"
#include "Game/Graphics/Graphics.h"

SelectionManager* SelectionManager::instance = NULL;

void SelectionManager::Render () {
    Graphics::BindTexture(NULL);
    FOR_(set<Entity*>::iterator, i, m_selectedEntities) {
        Entity* entity = *i;
        b2AABB aabb;
        aabb.lowerBound.x = 0;
        aabb.lowerBound.y = 0;
        aabb.upperBound.x = 0;
        aabb.upperBound.y = 0;

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
        Graphics::SetColor(Volt::Color::RGB(200, 200, 200));
        Graphics::RenderLineRect(0, 0, extents.x, extents.y);
        glPopMatrix();
    }
}
