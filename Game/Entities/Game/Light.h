#pragma once

#include "Game/Core/Core.h"
#include "Game/Editor/EntityFactory.h"
#include "Game/Game/Entity.h"
#include "Game/Graphics/Graphics.h"

namespace Volt {
    class GpuProgram;
}

class Light : public Entity {
public:
    DECLARE_ENTITY_(Light);

    Light ();
    virtual ~Light ();

    virtual Light* Clone () const;
    void CopyFrom (const Light* other);

    virtual void Update ();
    virtual void Render ();

    virtual void Load (const Json::Value& node);
    virtual void Save (Json::Value& node) const;

    virtual void OnScaleChanged ();
    virtual void GetProperties (vector<Property*>* properties);

    void ReloadShader ();

private:
    void CreatePhysicsBody ();

    Volt::Color m_color;
    float m_intensity;
    GLuint m_fbo;
    GLuint m_dummyTexture;
    GLuint m_shadowTexture;
    GLuint m_distanceTexture;
    GLuint m_parabolicTexture;
    GLuint m_lightTexture;
    Volt::GpuProgram* m_program;
    Volt::GpuProgram* m_program2;
    Volt::GpuProgram* m_program3;
    Volt::GpuProgram* m_program4;
};
