#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "overkill/graphics_internal/VertexArray.hpp"

struct SceneNode {
    ~SceneNode();
    // A list of all children that belong to this node.
    // For instance, in case of the scene graph of a human body shown in the assignment text, the "Upper Torso" node would contain the "Left Arm", "Right Arm", "Head" and "Lower Torso" nodes in its list of children.
    std::vector<SceneNode*> children;

    // The node's position and rotation relative to its parent
    glm::vec3 position = { 0,0,0 };
    glm::vec3 rotation = { 0,0,0 };

    // A transformation matrix representing the transformation of the node's location relative to its parent. This matrix is updated every frame.
    glm::mat4 matTRS = glm::mat4(1);

    // The location of the node's reference point
    glm::vec3 referencePoint = { 0,0,0 }; // node pivot

    // The ID of the VAO containing the "appearance" of this SceneNode.
    VertexArray vertexArray;
    unsigned int VAOIndexCount = 0;

    virtual void update(float deltaTime);

    void addChild(SceneNode* child);
    void printNode();
};

