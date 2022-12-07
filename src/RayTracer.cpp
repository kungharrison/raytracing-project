#include "Ray.h"
#include <string>

void RayTracer::Raytrace(Camera cam, RTScene &scene, Image &image) {
    int w = image.width; int h = image.height;
    for (int j=0; j<h; j++){
        for (int i=0; i<w; i++){
            Ray ray = RayThruPixel( cam, i, j, w, h );
            Intersection hit = Intersect( ray, scene );
            image.pixels[j * w + i] = FindColor(hit, scene, 1);
        }
    }
}

Ray RayTracer::RayThruPixel(Camera cam, int i, int j, int width, int height) {
    Ray ray;
    ray.p0 = cam.eye;
    float alpha = (2.0f * ((i + 0.5f) / width)) - 1;
    float beta = 1 - (2.0f * ((j + 0.5f) / height));
    float tan = glm::tan(glm::radians(cam.fovy) / 2.0f);
    glm::vec3 w = glm::normalize(cam.eye - cam.target);
    glm::vec3 u = glm::normalize(glm::cross(cam.up, w));
    glm::vec3 v = -glm::cross(w, u);
    ray.dir = glm::normalize(alpha * cam.aspect * tan * u + beta * tan * v - w);
    return ray;
}

Intersection RayTracer::Intersect(Ray ray, Triangle &triangle) {
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
        hit.P = y.x * triangle.P[0] + y.y * triangle.P[1] + y.z * triangle.P[2];
        hit.N = glm::normalize(y.x * triangle.N[0] + y.y * triangle.N[1] + y.z * triangle.N[2]);
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
        // std::cout << t.N[0].x << t.N[0].y << t.N[0].z << std::endl;
        // std::cout << t.N[1].x << t.N[1].y << t.N[1].z << std::endl;
        // std::cout << t.N[2].x << t.N[2].y << t.N[2].z << std::endl << std::endl;
        Intersection hit_temp = Intersect(ray, t);
        if (hit_temp.dist < mindist) { // closer than previous hit
            mindist = hit_temp.dist;
            hit = hit_temp;
        }
    }
    return hit;
}

glm::vec3 RayTracer::FindColor(Intersection &hit, RTScene &scene, int recursion_depth) {
    glm::vec3 color = glm::vec3(0.1f, 0.1f, 0.6f); // BG color of black
    if (recursion_depth <= 0) return color; // Base case
    if (hit.dist < std::numeric_limits<float>::infinity()) {
        color = glm::vec3(hit.triangle.material->ambient);
        // Loop through every light source
        for (int i = 0; i < scene.shader->lightpositions.size(); i++) {
            // Diffuse
            for (int j = 0; j < scene.light.size(); j++) {
                color += glm::vec3(hit.triangle.material->diffuse * scene.shader->lightcolors[j]) * glm::max(dot(hit.N, glm::vec3(scene.shader->lightpositions[j])), 0.0f);
            }
            Ray ray_to_light;
            ray_to_light.p0 = hit.P*1.03f;
            ray_to_light.dir = normalize(glm::vec3(scene.shader->lightpositions[i]) - hit.P);
            Intersection obstructed = Intersect( ray_to_light, scene );
            if (obstructed.dist == std::numeric_limits<float>::infinity()) {
                // Specular
                Ray reflected_ray;
                reflected_ray.p0 = hit.P*1.03f;
                reflected_ray.dir = 2.0f * dot(hit.N, hit.V) * hit.N - hit.V;
                Intersection reflected = Intersect(reflected_ray, scene);
                color += glm::vec3(hit.triangle.material->specular) * FindColor(reflected, scene, recursion_depth-1);
            }
        }
    }
    return color;
}
