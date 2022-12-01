#include "Ray.h"

void RayTracer::Raytrace(Camera cam, Scene scene, Image &image) {
    int w = image.width; int h = image.height;
    for (int j=0; j<h; j++){
        for (int i=0; i<w; i++){
            Ray ray = RayThruPixel( cam, i, j, w, h );
            Intersection hit = Intersect( ray, scene );
            image.pixels[j + i * h] = FindColor(hit, 1);
        }
    }
}

Ray RayTracer::RayThruPixel(Camera cam, int i, int j, int width, int height) {
    Ray ray;
    ray.p0 = cam.eye;
    float alpha = (2.0f * (i + 0.5f) / width) - 1;
    float beta = 1 - (2.0f * (j + 0.5f) / height);
    float tan = glm::tan(cam.fovy / 2.0f);
    glm::vec3 w = normalize(cam.eye - cam.target);
    glm::vec3 u = normalize(cam.up * w);
    glm::vec3 v = w * u;
    ray.dir = normalize(alpha * cam.aspect * tan * u + beta * tan * v - w);
    return ray;
}