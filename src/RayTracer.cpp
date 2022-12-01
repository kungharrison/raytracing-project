#include "Ray.h"

void RayTracer::Raytrace(Camera cam, RTScene scene, Image &image) {
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

Intersection RayTracer::Intersect(Ray ray, Triangle triangle) {
    Intersection hit;
    glm::mat4 x = inverse(glm::mat4(glm::vec4(triangle.P[0], 1), glm::vec4(triangle.P[1], 1), glm::vec4(triangle.P[2], 1), glm::vec4(-ray.dir, 0)));
    glm::vec4 y = x * glm::vec4(ray.p0, 1);
    hit.dist = y[3];
    hit.P = y[0] * triangle.P[0] + y[1] * triangle.P[1] + y[2] * triangle.P[2];
    hit.N = normalize(y[0] * triangle.N[0] + y[1] * triangle.N[1] + y[2] * triangle.N[2]);
    hit.triangle = &triangle;
    hit.V = ray.dir;
    return hit;
}

Intersection RayTracer::Intersect(Ray ray, RTScene scene) {
    float mindist = std::numeric_limits<float>::infinity();
    Intersection hit;
    for (Triangle t: scene.triangle_soup) { // Find closest intersection; test all objects
        Intersection hit_temp = Intersect(ray, t);
        if (hit_temp.dist < mindist) { // closer than previous hit
            mindist = hit_temp.dist;
            hit = hit_temp;
        }
    }
    return hit;
}

glm::vec3 RayTracer::FindColor(Intersection hit, int recursion_depth) {
    glm::vec3 color = glm::vec3(0, 0, 0);
    if (!hit.hit) return glm::vec3(0, 0, 0);

    if (hit.material->texture) {
        color = hit.material->texture->GetColor(hit.texCoord.x, hit.texCoord.y);
    }
    else {
        color = hit.material->diffuse;
    }

    if (hit.material->reflective && recursion_depth < 2) {
        Ray reflectRay;
        reflectRay.p0 = hit.point;
        reflectRay.dir = glm::normalize(hit.dir - 2 * glm::dot(hit.dir, hit.normal) * hit.normal);
        Intersection reflectHit = Intersect(reflectRay, *hit.scene);
        color += hit.material->reflective * FindColor(reflectHit, recursion_depth + 1);
    }
    return color;
}
