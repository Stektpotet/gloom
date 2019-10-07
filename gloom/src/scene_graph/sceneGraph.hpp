#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <stack>
#include <cstdio>
#include <stdbool.h>
#include <cstdlib> 
#include <ctime> 
#include <chrono>
#include <fstream>

#include "../overkill/graphics_internal/VertexArray.hpp"

// Matrix stack related functions
std::stack<glm::mat4>* createEmptyMatrixStack();
void pushMatrix(std::stack<glm::mat4>* stack, glm::mat4 matrix);
void popMatrix(std::stack<glm::mat4>* stack);
glm::mat4 peekMatrix(std::stack<glm::mat4>* stack);

void printMatrix(glm::mat4 matrix);