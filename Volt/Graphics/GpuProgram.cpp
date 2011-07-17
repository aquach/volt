#include "Volt/Graphics/GpuProgram.h"
#include "Volt/Graphics/OpenGL.h"
#include "Volt/Graphics/Graphics.h"

namespace Volt {

set<GpuProgram*> GpuProgram::s_programs;

GpuProgram::GpuProgram ()
    : m_linked(false) {
    if (glCreateProgramObjectARB == NULL) {
        LOG(FATAL) << "Graphics card does not support shaders.";
        return;
    }

    m_handle = glCreateProgramObjectARB();
    s_programs.insert(this);
}

GpuProgram::~GpuProgram () {
    UnloadShaders();
    s_programs.erase(this);
}

void GpuProgram::UnloadShaders () {
    for (uint i = 0; i < m_shaders.size(); i++) {;
        glDetachObjectARB(m_handle, m_shaders[i]->handle());
    }
    glDeleteObjectARB(m_handle);
}

void GpuProgram::Attach (ShaderAssetRef shader) {
    m_shaders.push_back(shader);
    glAttachObjectARB(m_handle, shader->handle());
    m_linked = false;
}

void GpuProgram::ReloadShaders () {
    m_handle = glCreateProgramObjectARB();
    for (uint i = 0; i < m_shaders.size(); i++) {
        glAttachObjectARB(m_handle, m_shaders[i]->handle());
    }
    m_linked = false;
}

bool GpuProgram::Link () {
    glLinkProgramARB(m_handle);

    GLint linkStatus = 0;
    glGetObjectParameterivARB(m_handle,
                              GL_OBJECT_LINK_STATUS_ARB,
                              &linkStatus);
    if (linkStatus == GL_FALSE) {
        int maxLength = 0;
        glGetObjectParameterivARB(m_handle,
                                  GL_OBJECT_INFO_LOG_LENGTH_ARB,
                                  &maxLength);

        char* log = new char[maxLength];
        int length;
        glGetInfoLogARB(m_handle, maxLength, &length, log);

        LOG(ERROR) << "GPU program '" << m_handle
                   << "' link failed : " << log;
        delete[] log;

        return false;
    }
    m_linked = true;
    return true;
}

void GpuProgram::UnloadAllShaders () {
    FOR_(set<GpuProgram*>::iterator, i, s_programs)
        (*i)->UnloadShaders();
}

void GpuProgram::ReloadAllShaders () {
    FOR_(set<GpuProgram*>::iterator, i, s_programs)
        (*i)->ReloadShaders();
}



}
