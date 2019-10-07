#pragma once
#include "SceneNode.hpp"

struct PathFollowingNode : public SceneNode
{
    double pathTime = 0;
    PathFollowingNode(double startTime = 0.0);
    void update(float deltaTime) override;
};