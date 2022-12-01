#include <glm/glm.hpp>
#include "Image.h"
#include "Camera.h"
#include "Scene.h"
#include "Triangle.h"
#include "Intersection.h"

#ifndef __RAY_H__
#define __RAY_H__

class Ray {
    glm::vec3 p0; // basepoint
    glm::vec3 dir; // direction
};

namespace RayTracer {
    void Raytrace(Camera cam, Scene scene, Image &image); //page 9
    Ray RayThruPixel(Camera cam, int i, int j, int width, int height);//page 10,18
    Intersection Intersect(Ray ray, Triangle triangle); //page 30, 33
    Intersection Intersect(Ray ray, Scene scene); //page 11, 28, 31
    glm::vec3 FindColor(Intersection hit, int recursion_depth); //page 15
};

#endif 
