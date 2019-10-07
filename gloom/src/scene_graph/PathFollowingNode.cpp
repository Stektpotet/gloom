#include "PathFollowingNode.hpp"
#include "toolbox.hpp"

PathFollowingNode::PathFollowingNode(double startTime)
    :   pathTime{startTime}
{}

void PathFollowingNode::update(float deltaTime)
{
    pathTime += deltaTime*0.75;
    Heading heading = simpleHeadingAnimation(pathTime);
    position = glm::vec3{ heading.x, 10.0f, heading.z };
    rotation = glm::vec3{ heading.pitch, heading.yaw, heading.roll };
}
