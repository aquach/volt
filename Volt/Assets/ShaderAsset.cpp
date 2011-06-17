#include "ShaderAsset.h"
#include "AssetManager.h"
#include "DataSource.h"
#include "Graphics/OpenGL.h"

namespace Volt {

class Shader {
public:
    Shader (ShaderAsset::ShaderType type)
        : m_handle(-1),
          m_type(type) {
        if (type == ShaderAsset::SHADER_VERTEX)
            m_handle = glCreateShaderObjectARB(GL_VERTEX_SHADER);
        else if (type == ShaderAsset::SHADER_FRAGMENT)
            m_handle = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);
        else
            LOG(WARNING) << "Unknown shader type.";
    }

    ~Shader () {
        glDeleteObjectARB(m_handle);
    }

    void Load (const DataItem& item) {
        char* buffer = new char[item.size + 1];
        buffer[item.size] = 0;
        memcpy(buffer, item.data, item.size);
        glShaderSourceARB(m_handle, 1, (const char**)&buffer, NULL);
        m_filename = item.path;
        free(buffer);
    }

    bool Compile () {
        LOG(INFO) << "Compiling " << m_filename << "...";
        glCompileShaderARB(m_handle);

        GLint compileStatus = 0;
        glGetObjectParameterivARB(m_handle,
                                  GL_OBJECT_COMPILE_STATUS_ARB,
                                  &compileStatus);
        if (compileStatus == GL_FALSE) {
            int maxLength = 0;
            glGetObjectParameterivARB(m_handle,
                                      GL_OBJECT_INFO_LOG_LENGTH_ARB,
                                      &maxLength);

            char* log = new char[maxLength];
            int length;
            glGetInfoLogARB(m_handle, maxLength, &length, log);

            LOG(ERROR) << " Shader compilation failed for '"
                       << m_filename << "' :" << log;
            delete[] log;
            return false;
        }

        return true;
    }

    ShaderAsset::ShaderType m_type;
    GLuint m_handle;
    string m_filename;
};

ShaderAsset::~ShaderAsset () {
    if (m_shader != NULL)
        delete m_shader;
}

int ShaderAsset::handle () const {
    return m_shader->m_handle;
}

bool ShaderAsset::Load (const DataItem& item, ShaderType type) {
    m_shader = new Shader(type);
    m_shader->Load(item);
    m_path = item.path;
    m_type = type;
    return m_shader->Compile();
}

void ShaderAsset::Reload () {
    Unload();
    DataItem item;
    m_manager->ReloadPath(this, &item);
    Load(item, m_type);
}

void ShaderAsset::Unload () {
    delete m_shader;
    m_shader = NULL;
}

}
