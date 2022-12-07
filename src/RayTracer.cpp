#include "Ray.h"

void RayTracer::Raytrace(Camera cam, RTScene &scene, Image &image) {
    int w = image.width; int h = image.height;
    for (int j=0; j<h; j++){
        for (int i=0; i<w; i++){
            Ray ray = RayThruPixel( cam, i, j, w, h );
            Intersection hit = Intersect( ray, scene );
            image.pixels[j * w + i] = FindColor(hit, 1);
        }
    }
}

Ray RayTracer::RayThruPixel(Camera cam, int i, int j, int width, int height) {
    Ray ray;
    ray.p0 = cam.eye;
    float alpha = (2.0f * ((i + 0.5f) / width)) - 1;
    float beta = 1 - (2.0f * ((j + 0.5f) / height));
    float tan = glm::tan(glm::radians(cam.fovy) / 2.0f);
    glm::vec3 w = normalize(cam.eye - cam.target);
    glm::vec3 u = normalize(cross(cam.up, w));
    glm::vec3 v = cross(w, u);
    ray.dir = normalize(alpha * cam.aspect * tan * u + beta * tan * v - w);
    return ray;
}

Intersection RayTracer::Intersect(Ray ray, Triangle triangle) {
    Intersection hit;
    glm::vec4 cols[4];
    cols[0] = glm::vec4(triangle.P[0], 1.0f);
    cols[1] = glm::vec4(triangle.P[1], 1.0f);
    cols[2] = glm::vec4(triangle.P[2], 1.0f);
    cols[3] = glm::vec4(-ray.dir, 0.0f);
    glm::mat4 h = glm::mat4(cols[0], cols[1], cols[2], cols[3]);
    glm::mat4 x = glm::inverse(h);
    glm::vec4 y = x * glm::vec4(ray.p0, 1.0f);
    if (y[0] < 0 || y[1] < 0 || y[2] < 0 || y[3] < 0) {
        hit.dist = std::numeric_limits<float>::infinity();
    } else {
        hit.dist = y.w;
        hit.P = y[0] * triangle.P[0] + y[1] * triangle.P[1] + y[2] * triangle.P[2];
        hit.N = normalize(y[0] * triangle.N[0] + y[1] * triangle.N[1] + y[2] * triangle.N[2]);
        hit.triangle = triangle;
        hit.V = -ray.dir;
    }
    return hit;
}

Intersection RayTracer::Intersect(Ray ray, RTScene &scene) {
    float mindist = std::numeric_limits<float>::infinity();
    Intersection hit;
    hit.dist = mindist;
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
    glm::vec3 color = glm::vec3(1, 1, 1);
    if (hit.dist < std::numeric_limits<float>::infinity()) {
        color = glm::vec3(0, 0, 0);
    }
    // if (!hit.hit) return glm::vec3(0, 0, 0);

    // if (hit.material->texture) {
    //     color = hit.material->texture->GetColor(hit.texCoord.x, hit.texCoord.y);
    // }
    // else {
    //     color = hit.material->diffuse;
    // }

    // if (hit.material->reflective && recursion_depth < 2) {
    //     Ray reflectRay;
    //     reflectRay.p0 = hit.point;
    //     reflectRay.dir = glm::normalize(hit.dir - 2 * glm::dot(hit.dir, hit.normal) * hit.normal);
    //     Intersection reflectHit = Intersect(reflectRay, *hit.scene);
    //     color += hit.material->reflective * FindColor(reflectHit, recursion_depth + 1);
    // }
    return color;
}
