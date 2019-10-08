#pragma once
#include <glad/glad.h>
#include <vector>
#include <unordered_map>
#include <initializer_list>
#include "../gfx.h"

struct UniformAttribute 
{
    char* m_name;
    GLuint size;
}

struct UniformBlockLayout
{
    char* m_name;
    std::vector<UniformAttribute> m_attributes;
};

class UniformBuffer
{
private:
    GL
    GLuint m_id;
    char* m_name;
    std::unordered_map<char*, GLuint> m_uniforms;
public:

    UniformBuffer(const char *name, std::initializer_list<std::pair<const GLsizeiptr, const void*>> data);

    GLuint ID() const;

    void clean();

    void bind() const;
    void unbind() const;

    inline GLuint blockSize()
    {
        return m_blockLayout.size();
    }

    GLuint getUniformIndex(const C::Tag& name) const;
    void update(const C::ID index, GLsizeiptr size, const void *data);
};

}
