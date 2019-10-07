#pragma once
#include <vector>
#include <unordered_map>
#include <set>
#include "../gfx.h"

#include "VertexLayout.hpp"

class VertexArray
{
public:
	VertexArray();

	inline bool valid() const { return id != 0; }
	inline GLuint ID() const { return id; }

    inline void bind() const
    {
        GFX_GL_CALL(glBindVertexArray(id));
    }
    static inline void unbind()
    {
        GFX_GL_CALL(glBindVertexArray(0));
    }
    inline void clear()
    {
        GFX_DEBUG("VAO (%d) deleting...", id);
        glDeleteVertexArrays(1, &id);
    }

private:
	GLuint id = 0;

	//std::unordered_map<GLuint> buffers();
    std::unordered_map<VertexLayout*, std::set<VertexBuffer>> layouts;
	//list layouts, let buffers subscribe to one of them
	//std::vector<VertexLayout> layouts; 
};