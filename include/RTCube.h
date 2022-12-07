#include "RTGeometry.h"
#include "Triangle.h"
#ifndef __RTCUBE_H__
#define __RTCUBE_H__

class RTCube : public RTGeometry {
public:
    void init(void) {
        // vertex positions
        const GLfloat positions[24][3] = {
            // Front face
            { -0.5f, -0.5f, 0.5f },{ -0.5f, 0.5f, 0.5f },{ 0.5f, 0.5f, 0.5f },{ 0.5f, -0.5f, 0.5f },
            // Back face
            { -0.5f, -0.5f, -0.5f },{ -0.5f, 0.5f, -0.5f },{ 0.5f, 0.5f, -0.5f },{ 0.5f, -0.5f, -0.5f },
            // Left face
            { -0.5f, -0.5f, 0.5f },{ -0.5f, 0.5f, 0.5f },{ -0.5f, 0.5f, -0.5f },{ -0.5f, -0.5f, -0.5f },
            // Right face
            { 0.5f, -0.5f, 0.5f },{ 0.5f, 0.5f, 0.5f },{ 0.5f, 0.5f, -0.5f },{ 0.5f, -0.5f, -0.5f },
            // Top face
            { 0.5f, 0.5f, 0.5f },{ -0.5f, 0.5f, 0.5f },{ -0.5f, 0.5f, -0.5f },{ 0.5f, 0.5f, -0.5f },
            // Bottom face
            { 0.5f, -0.5f, 0.5f },{ -0.5f, -0.5f, 0.5f },{ -0.5f, -0.5f, -0.5f },{ 0.5f, -0.5f, -0.5f }
        };
        // vertex normals
        const GLfloat normals[24][3] = {
            // Front face
            { 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },
            // Back face
            { 0.0f, 0.0f, -1.0f },{ 0.0f, 0.0f, -1.0f },{ 0.0f, 0.0f, -1.0f },{ 0.0f, 0.0f, -1.0f },
            // Left face
            { -1.0f, 0.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },
            // Right face
            { 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },
            // Top face
            { 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },
            // Bottom face
            { 0.0f, -1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f }
        };

        // convert above GLfloats into vector of all triangles
        		
        for (int i = 0; i < 24; i+=4) {
            Triangle *newTriangle = new Triangle();
            Triangle *newTriangle2 = new Triangle();
            newTriangle->P.push_back(glm::vec3(positions[i][0], positions[i][1], positions[i][2]));
            newTriangle->P.push_back(glm::vec3(positions[i+1][0], positions[i+1][1], positions[i+1][2]));
            newTriangle->P.push_back(glm::vec3(positions[i+2][0], positions[i+2][1], positions[i+2][2]));
            newTriangle->N.push_back(glm::vec3(normals[i][0], normals[i][1], normals[i][2]));
            newTriangle->N.push_back(glm::vec3(normals[i + 1][0], normals[i + 1][1], normals[i + 1][2]));
            newTriangle->N.push_back(glm::vec3(normals[i + 2][0], normals[i + 2][1], normals[i + 2][2]));
            newTriangle2->P.push_back(glm::vec3(positions[i+1][0], positions[i+1][1], positions[i+1][2]));
            newTriangle2->P.push_back(glm::vec3(positions[i + 2][0], positions[i + 2][1], positions[i + 2][2]));
            newTriangle2->P.push_back(glm::vec3(positions[i + 3][0], positions[i + 3][1], positions[i + 3][2]));
            newTriangle2->N.push_back(glm::vec3(normals[i+1][0], normals[i+1][1], normals[i+1][2]));
            newTriangle2->N.push_back(glm::vec3(normals[i + 2][0], normals[i + 2][1], normals[i + 2][2]));
            newTriangle2->N.push_back(glm::vec3(normals[i + 3][0], normals[i + 3][1], normals[i + 3][2]));
            elements.push_back(*newTriangle);
            elements.push_back(*newTriangle2);
        }
    };
};
#endif