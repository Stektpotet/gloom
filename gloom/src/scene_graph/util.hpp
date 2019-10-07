#pragma once
#include "../mesh.hpp"
#include "SceneNode.hpp"

// Creates an empty SceneNode instance.
template<typename TNode, typename... Args>
TNode* createSceneNode(Args... ctorArgs) {
    return new TNode(std::forward<Args>(ctorArgs)...);
}

// Transfer the mesh to the GPU
template<typename TNode>
void TransferMesh(Mesh& mesh, TNode* node)
{
    node->vertexArray.bind();
    GLuint
        positionsByteSize = static_cast<GLuint>(mesh.vertices.size() * sizeof(float)),
        normalsByteSize = static_cast<GLuint>(mesh.normals.size() * sizeof(float)),
        colorsByteSize = static_cast<GLuint>(mesh.colours.size() * sizeof(float));

    VertexBuffer vbo = { //create a vbo with enough space for all the attributes
        static_cast<GLsizeiptr>(positionsByteSize + normalsByteSize + colorsByteSize)
    };

    vbo.update( //upload the positional data
               0, positionsByteSize, mesh.vertices.data()
    );
    vbo.update( //upload the normal data
               positionsByteSize, normalsByteSize, mesh.normals.data()
    );
    vbo.update( //upload the color data
               positionsByteSize + normalsByteSize, colorsByteSize, mesh.colours.data()
    );

    IndexBuffer<unsigned int> ibo = {
        static_cast<GLsizeiptr>(mesh.indices.size()), mesh.indices.data()
    };

    ContinuousVertexLayout{ //Attribute layout descriptor
        // name, size, components, internal type, normalized
        {"position", positionsByteSize, 3, GL_FLOAT, GL_FALSE},
        {"normal", normalsByteSize, 3, GL_FLOAT, GL_TRUE},
        {"color", colorsByteSize, 4, GL_FLOAT, GL_TRUE},
    }.applyToBuffer(vbo); //Activate the given attributes


    node->VAOIndexCount = mesh.indices.size();
}


void updateTransforms(SceneNode* node, glm::mat4 transformationThusFar = glm::mat4(1))
{
    // Do transformation computations here
    glm::mat4 model = glm::translate(glm::mat4(1),node->referencePoint);  //move to reference point
    model = glm::rotate(model, node->rotation.x, { 1, 0, 0 }); //Rotate relative to referencePoint
    model = glm::rotate(model, node->rotation.y, { 0, 1, 0 });
    model = glm::rotate(model, node->rotation.z, { 0, 0, 1 });
    model = glm::translate(model, -(node->referencePoint));  //move back into model space
    model = glm::translate(node->position) * model;
    node->matTRS = transformationThusFar * model;
    //model = glm::translate(model, node->position);                  //Translate relative to referencePoint

    // Store matrix in the node's currentTransformationMatrix here

    //node->matTRS = transformationThusFar * model;

    for (const auto& child : node->children)
        updateTransforms(child, node->matTRS);
}
