#include "rotorNode.hpp"

RotorNode::RotorNode(glm::vec3 rotationAxis /*= glm::vec3{1,0,0}*/, float rotorSpeed /*= 10.0f*/)
  : axis{ glm::normalize(rotationAxis) },
    rotorSpeed{ rotorSpeed }
{}

void RotorNode::update(float deltaTime)
{
    rotation = rotation + (rotorSpeed * axis) * deltaTime;
}