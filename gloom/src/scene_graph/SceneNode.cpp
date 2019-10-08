#include "SceneNode.hpp"

SceneNode::~SceneNode()
{
    vertexArray.clear(); //release the vao from the GPU
    shader.destroy();
}


void SceneNode::update(float deltaTime) {}

// Add a child node to its parent's list of children
void SceneNode::addChild(SceneNode* child) {
    children.push_back(child);
}

// Pretty prints the current values of a SceneNode instance to stdout
void SceneNode::printNode() {
    GFX_INFO(
        "SceneNode {\n"
        "    Child count: %d\n"
        "    Rotation: (%f, %f, %f)\n"
        "    Location: (%f, %f, %f)\n"
        "    Reference point: (%f, %f, %f)\n"
        "    VAO ID: %d\n"
        "}\n",
        children.size(),
        rotation.x, rotation.y, rotation.z,
        position.x, position.y, position.z,
        referencePoint.x, referencePoint.y, referencePoint.z,
        vertexArray.ID());
}

