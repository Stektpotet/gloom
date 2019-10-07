#include "sceneGraph.hpp"
#include <iostream>

// --- Matrix Stack related functions ---

// You can use these to create a more "realistic" scene graph implementation 

// Allocate a new empty matrix stack on the heap
std::stack<glm::mat4>* createEmptyMatrixStack() {
	return new std::stack<glm::mat4>();
}

// Push a matrix on top of the stack
void pushMatrix(std::stack<glm::mat4>* stack, glm::mat4 matrix) {
	stack->push(matrix);
}

// Remove a matrix from the top of the stack. The popped value is not returned.
void popMatrix(std::stack<glm::mat4>* stack) {
	stack->pop();
}

// Return the matrix which is currently at the top of the stack
glm::mat4 peekMatrix(std::stack<glm::mat4>* stack) {
	return stack->top();
}

// Pretty prints the values of a matrix to stdout. 
void printMatrix(glm::mat4 matrix) {
	float* values = glm::value_ptr(matrix);

	printf("(%f, %f, %f, %f)\n(%f, %f, %f, %f)\n(%f, %f, %f, %f)\n(%f, %f, %f, %f)\n",
		values[0], values[4], values[8], values[12], 
		values[1], values[5], values[9], values[13], 
		values[2], values[6], values[10], values[14], 
		values[3], values[7], values[11], values[15]);
}

