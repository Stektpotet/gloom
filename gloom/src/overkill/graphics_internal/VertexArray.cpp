#include "VertexArray.hpp"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
    GFX_DEBUG("VAO (%d) bound", id);
}