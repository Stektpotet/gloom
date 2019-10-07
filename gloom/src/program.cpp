#include <algorithm>

// Local headers
#include "gloom/program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"

#include <glm/mat4x4.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtx/transform.hpp> 
#include <glm/vec3.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "overkill/ShaderLoader.hpp"
#include "overkill/graphics_internal/VertexArray.hpp"
#include "overkill/graphics_internal/VertexBuffer.hpp"
#include "overkill/graphics_internal/IndexBuffer.hpp"
#include "overkill/graphics_internal/VertexLayout.hpp"
#include "overkill/graphics_internal/ShaderProgram.hpp"

#include "scene_graph/SceneNode.hpp"
#include "scene_graph/RotorNode.hpp"
#include "scene_graph/PathFollowingNode.hpp"
#include "scene_graph/util.hpp"
#include "OBJLoader.hpp"
#include "mesh.hpp"

#include "toolbox.hpp"

float cam_pitch;
float cam_yaw;
glm::vec3 cam_lookDirection = glm::vec3(0,0,1);
glm::vec3 cam_right = glm::vec3(1, 0, 0);
glm::vec3 cam_up = glm::vec3(0, 1, 0);
glm::vec3 cam_position = glm::vec3(0.0f,0.0f,0.0f);
const float CAM_SPEED = 50.0f;
const float CAM_ROTATION_SPEED = 1.0f;
double dDeltaTime = 0;

SceneNode* constructScene()
{
    Mesh terrainMesh = loadTerrainMesh("../gloom/res/models/lunarsurface.obj");
    Helicopter heliModel = loadHelicopterModel("../gloom/res/models/helicopter.obj");

    SceneNode
        *terrain = createSceneNode<SceneNode>(),
        *heliBody = createSceneNode<PathFollowingNode>(3.0),
        *heliDoor = createSceneNode<SceneNode>(),
        *heliMainRotor = createSceneNode<RotorNode>(glm::vec3{0,1,0}, 15.0f),
        *heliTailRotor = createSceneNode<RotorNode>(glm::vec3{1,0,0}, -45.0f);

    TransferMesh(terrainMesh, terrain),
    TransferMesh(heliModel.body, heliBody),
    TransferMesh(heliModel.door, heliDoor);
    TransferMesh(heliModel.mainRotor, heliMainRotor),
    TransferMesh(heliModel.tailRotor, heliTailRotor);

    heliMainRotor->referencePoint = { 0.0f,1.5f,0.0f };
    heliTailRotor->referencePoint = { 0.35f,2.3f,10.4f };

    auto root = createSceneNode<SceneNode>();
        root->addChild(terrain);
        root->addChild(heliBody);
            heliBody->addChild(heliDoor);
            heliBody->addChild(heliMainRotor);
            heliBody->addChild(heliTailRotor);

    updateTransforms(root);
    return root;
}

void renderNode(const SceneNode* node, glm::mat4 viewProjection, ShaderProgram boundShader)
{
    // All renderable objects has a VAO, meaning it's id will not be -1, all VAOs with id != 0 are considered valid.
    if(node->vertexArray.valid()) 
    {
        //Slightly inefficient to have a lookup of uniforms every for frame... but for now, it's an easy approach
        glUniformMatrix4fv(boundShader.getUniformLocation("mvp"), 1, GL_FALSE, glm::value_ptr(viewProjection * node->matTRS));
        glUniformMatrix4fv(boundShader.getUniformLocation("m2w"), 1, GL_FALSE, glm::value_ptr(node->matTRS));
        node->vertexArray.bind();
        GFX_GL_CALL(glDrawElements(GL_TRIANGLES, node->VAOIndexCount, GL_UNSIGNED_INT, 0));
    }
    for (const auto& child : node->children)
        renderNode(child, viewProjection, boundShader);
}

void animate(float deltaTime, SceneNode* node)
{
    node->update(deltaTime);
    for (const auto& child : node->children)
        animate(deltaTime, child);
}

void runProgram(GLFWwindow* window)
{
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);


    glm::vec4 tint = { 0.6,0.8,1,1 };

    SceneNode* scene = constructScene();

    //Create the shader to draw the geometry
    ShaderProgram shader = createProgram("../gloom/res/shaders/simple.vert", "../gloom/res/shaders/simple.frag");

    GLint u_timeHandle =    shader.getUniformLocation("time");
    GLint u_viewMatHandle = shader.getUniformLocation("view");
    GLint u_projMatHandle = shader.getUniformLocation("projection");
    GLint u_camPosHandle = shader.getUniformLocation("view_wPosition"); //camera world position

    double dLastTime = 0;
    double dTimeSinceStartup = 0;
    double dTime = 0;


    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        dTimeSinceStartup = glfwGetTime();
        dDeltaTime = dTimeSinceStartup - dLastTime;
        if (glfwGetKey(window, GLFW_KEY_SPACE))
            dTime += dDeltaTime;
            glUniform1f(u_timeHandle, dTime);
        // Draw your scene here
            
        glm::mat4 mat_view = glm::rotate(glm::mat4(1), cam_pitch, glm::vec3{ 1,0,0 });
        mat_view = glm::rotate(mat_view, cam_yaw, glm::vec3{ 0,1,0 });
        mat_view = glm::translate(mat_view, -cam_position);
        glm::mat4 mat_proj = glm::perspective(glm::radians(75.0f), (float)windowHeight / windowWidth, 0.2f, 1500.0f);
        
        glUniformMatrix4fv(u_viewMatHandle, 1, GL_FALSE, glm::value_ptr(mat_view));
        glUniformMatrix4fv(u_projMatHandle, 1, GL_FALSE, glm::value_ptr(mat_proj));
        glUniform3fv(u_camPosHandle, 1, glm::value_ptr(cam_position));

        animate((float)dDeltaTime, scene);
        updateTransforms(scene);
        renderNode(scene, mat_proj * mat_view, shader);

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);
        // Flip buffers
        glfwSwapBuffers(window);

        dLastTime = dTimeSinceStartup;
    }
}

void handleKeyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cam_pitch -= CAM_ROTATION_SPEED *(float)dDeltaTime;
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cam_pitch += CAM_ROTATION_SPEED * (float)dDeltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            cam_yaw -= CAM_ROTATION_SPEED * (float)dDeltaTime;
        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            cam_yaw += CAM_ROTATION_SPEED * (float)dDeltaTime;
        }

        //clamp rotation values to a range of 2pi
        if (cam_pitch > glm::two_pi<float>())
            cam_pitch -= glm::two_pi<float>();
        else if (cam_pitch < 0)
            cam_pitch = glm::two_pi<float>() + cam_pitch;

        if (cam_yaw > glm::two_pi<float>())
            cam_yaw -= glm::two_pi<float>();
        else if (cam_yaw < 0)
            cam_yaw = glm::two_pi<float>() + cam_yaw;

        cam_lookDirection.x = -glm::sin(cam_yaw) * glm::cos(cam_pitch);
        cam_lookDirection.y = sin(cam_pitch);
        cam_lookDirection.z = cos(cam_yaw) * cos(cam_pitch);

        cam_right = glm::normalize(glm::cross(glm::vec3{ 0,1,0 }, cam_lookDirection));
        cam_up = glm::cross(cam_lookDirection, cam_right);
    }
    else
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cam_position -= cam_lookDirection * CAM_SPEED * (float)dDeltaTime;
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cam_position += cam_lookDirection * CAM_SPEED * (float)dDeltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            cam_position -= cam_right * CAM_SPEED * (float)dDeltaTime;
        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            cam_position += cam_right * CAM_SPEED * (float)dDeltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            cam_position -= cam_up * CAM_SPEED * (float)dDeltaTime;
        }
        else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            cam_position += cam_up * CAM_SPEED * (float)dDeltaTime;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        //reset if you get lost :P
        cam_pitch = 0;
        cam_yaw = 0;
        cam_lookDirection = glm::vec3(0, 0, 1);
        cam_right = glm::vec3(1, 0, 0);
        cam_up = glm::vec3(0, 1, 0);
        cam_position = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
