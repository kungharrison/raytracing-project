# Raytracing Project Writeup
## Authors
Holden Adamec and Harrison Kung
## Overview
Raytracing Project for Computer Graphics 167. Modifies a basic diffuse and Blinn-Phong specular and ambient shading by replacing it with a recursive mirror reflection to generate raytraced graphics. This project uses raytracing through the creation of triangle geometry which uses rays from light sources to check for intersections with triangles and traces the light sources rays. 
## Topics

### Image

The image class contains the array of pixels where colors are stored, using a vec3 to store pixel colors. This pixel buffer is drawn to the screen after the running the ray tracer. The ray tracer fills every pixel in the array by shooting a ray from the camera through that pixel and seeing what the ray intersects with. Basically the image class just acts as a buffer we use to draw our image to the screen. Nothing fancy here.

### Triangle

The traingle header file stores two vectors of vec3s, one storing the three positions of the vertices making up the triangle, and the other storing the three normals of each vertex. It also stores a material for the triangle for findColor to use to color the geometry.

### Geometry

RTGeometry contains a list of elements representing all the triangles of the object, this is created through RTObj which uses a for loop to iterate through all positions and normals read in from the obj file and storing each three positions and normals as a triangle as described above in the Triangle header file.

### Scene

Scene does a few different things. In RTScene.inl, we create a graph that represents our scene. Every node in the scene has a list of models and a list of child nodes. Each of these has some matrices which transform it in relation to its parent.

In RTScene.cpp, we take this graph we use to represent the scene and traverse it using depth first search. This occurs in the build triangle soup function, the most important aspect of RTScene. Basically, it takes every triangle in the scene and applies the correct transformations so that it is represented in world space rather than in relation to any other models. Then we take all of the transformed triangles, assign them the proper material, and chuck them into a big unordered vector called triangle soup. We loop through all of these triangles in the ray tracer to check for intersections.

### Ray

Ray represents a single abstract mathematical ray. It consists of a position p0 and a direction dir.

### RayTracer (class declaration located in Ray.h)

This is the meat of the project where the magic happens.

The main function RayTrace is called before drawing the image to the screen. In it is a nested for loop that looks like this:

```cpp
for (int j=0; j<h; j++){
    for (int i=0; i<w; i++){
        Ray ray = RayThruPixel( cam, i, j, w, h );
        Intersection hit = Intersect( ray, scene );
        image.pixels[j * w + i] = FindColor(hit, scene, 10);
    }
}
```

This loops through every pixel on the sceen. The first line `Ray ray = RayThruPixel( cam, i, j, w, h );` casts a ray from our camera like in this example from lecture.

![image](https://user-images.githubusercontent.com/53415491/206325923-7ee704c7-3283-4c1d-8bc0-fa543efcd53b.png)

We are effectively trying to do the reverse of what happens in real life. In real life photons traveling in straight lines hit our eye from all directions. We could render scenes in this manner, creating a bunch of photons and shooting them out from a light source, however, most of them wouldn't even come close to hitting the camera and a lot of computation would be wasted. To do this more efficiently, we know run the process in reverse because we know that every photon that hits the camera at least has a chance of having come from a light source.

Now that we have a ray going through the current pixel starting at the camera, we want to see if that ray hits any objects in our scene. The next line of code `Intersection hit = Intersect( ray, scene );` finds the closest intersecting point of any geometry in our scene. We want the closest point because if a ray intersects multiple things, we will only see the thing closest to us (unless there is transparency which we aren't considering here).

The final line of code `image.pixels[j * w + i] = FindColor(hit, scene, 10);` runs our coloring model on the intersection. If the ray doesn't intersect anything, the model will just return a default background color. The number "10" just means that we have a max recursion depth of 10. When we draw intersection, it may also reflect light from other parts of our scene resulting in a reflected ray. The max depth just ensures that we don't have an infinite loop when recursing.

### RayThruPixel

Though mentioned above, there is a lot more that goes in to calculating the ray that insersects a pixel. The easy part is the ray position, which is just set to the same position as the camera, afterall that is where we cast the ray from. Calculating the direction is more involved, but its really still just math. The formula is found below:

![image](https://user-images.githubusercontent.com/53415491/206328243-88009e93-6c38-4747-98e9-69c9d7fe71b4.png)

Here is it implemented in code:

```cpp
float alpha = (2.0f * ((i + 0.5f) / width)) - 1;
float beta = 1 - (2.0f * ((j + 0.5f) / height));
float tan = glm::tan(glm::radians(cam.fovy) / 2.0f);
glm::vec3 w = glm::normalize(cam.eye - cam.target);
glm::vec3 u = glm::normalize(glm::cross(cam.up, w));
glm::vec3 v = -glm::cross(w, u);
ray.dir = glm::normalize(alpha * cam.aspect * tan * u + beta * tan * v - w);
```

### Intersect (triangle)

This function takes in a ray and triangle, and returns a hit. Barycentric coordinates make finding intersections easy. All we do is a little matrix math and we can find if there is an intersection and where exactly it occured.

![image](https://user-images.githubusercontent.com/53415491/206328505-0bd96d0f-d7ce-411c-87de-b4838ed6b6d8.png)

Code for this part is found below:

```cpp
// This is the matrix from the slides
glm::vec4 cols[4];
cols[0] = glm::vec4(triangle.P[0], 1.0f);
cols[1] = glm::vec4(triangle.P[1], 1.0f);
cols[2] = glm::vec4(triangle.P[2], 1.0f);
cols[3] = glm::vec4(-ray.dir, 0.0f);
glm::mat4 h = glm::mat4(cols[0], cols[1], cols[2], cols[3]);
// We invert the matrix
glm::mat4 x = glm::inverse(h);
// Calculate lambdas and distance t
glm::vec4 y = x * glm::vec4(ray.p0, 1.0f);
// Check for intersection
if (y[0] < 0 || y[1] < 0 || y[2] < 0 || y[3] < 0) {
    hit.dist = std::numeric_limits<float>::infinity();
} else {
    hit.dist = y.w;
    hit.P = y.x * triangle.P[0] + y.y * triangle.P[1] + y.z * triangle.P[2];
    hit.N = glm::normalize(y.x * triangle.N[0] + y.y * triangle.N[1] + y.z * triangle.N[2]);
    hit.triangle = triangle;
    hit.V = -ray.dir;
}
```

### Intersect (scene)

We don't actually find intersections here, we just run the triangle ray intersection function (above) for every triangle in our triangle soup. It only saves the closest hit point to the camera because that is all we care about rendering.

### FindColor

We start by setting a default background color `glm::vec3 color = glm::vec3(0.1f, 0.1f, 0.2f);`

Next we check base cases. If the hit distance is infinity it means we never hit anything and should return the default color. The same goes for when we hit the max recursion depth.

If there is a hit, then we set the color to the ambient material color. This is the default shadow color of the object.

Next we move on to shadows. We do this by drawing a ray from the hit position to the position of every light. We also add in an offset to prevent self-shadowing problems. We run the same intersection code on this ray to check if the point should be in shadow because it is obstructed by from the light source in question.

```cpp
Ray ray_to_light;
ray_to_light.p0 = hit.P*1.00002f;
ray_to_light.dir = normalize(glm::vec3(scene.shader->lightpositions[i]) - hit.P);
Intersection obstructed = Intersect( ray_to_light, scene );
```

If not obstructed we add in the diffuse lighting, calculated as follows:

```cpp
color += glm::vec3(hit.triangle.material->diffuse * scene.shader->lightcolors[j]) * glm::max(dot(hit.N, glm::vec3(scene.shader->lightpositions[j])), 0.0f);
```

Lastly we must calculate the reflections. This requires recursion. Basically we draw another ray pointing away from our surface and recurse with this ray if it intersects with anything in the scene. Then we add this to the color too, multiplying by the material's specular quality.

```cpp
 Ray reflected_ray;
reflected_ray.p0 = hit.P*1.00003f;
reflected_ray.dir = 2.0f * dot(hit.N, hit.V) * hit.N - hit.V;
Intersection reflected = Intersect(reflected_ray, scene);
glm::vec3 recurse = FindColor(reflected, scene, recursion_depth-1);
color += glm::vec3(hit.triangle.material->specular) * recurse;
```
