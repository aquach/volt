#include "GpuProgram.h"
#include "OpenGL.h"

namespace Volt {

GpuProgram::GpuProgram () {
    m_handle = glCreateProgramObjectARB();
}

GpuProgram::~GpuProgram () {
    for (unsigned int i = 0; i < m_shaders.size(); i++) {
        glDetachObjectARB(m_handle, m_shaders[i]->handle());
    }
    glDeleteObjectARB(m_handle);
}

void GpuProgram::Attach (ShaderAssetRef shader) {
    m_shaders.push_back(shader);
    glAttachObjectARB(m_handle, shader->handle());
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

        LOG(ERROR) << "Error, GPU program '" << m_handle
                   << "' link failed : " << log;
        delete[] log;

        return false;
    }
    return true;
}

}
