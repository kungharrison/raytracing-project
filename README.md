# Raytracing Project Writeup
## Authors
Holden Adamec and Harrison Kung
## Overview
Raytracing Project for Computer Graphics 167. Modifies a basic diffuse and Blinn-Phong specular and ambient shading by replacing it with a recursive mirror reflection to generate raytraced graphics. This project uses raytracing through the creation of triangle geometry which uses rays from light sources to check for intersections with triangles and traces the light sources rays. 
## Topics
### Image
The image class contains the array of pixels where colors are stored, using a vector 3 to store all pixels.
### Triangle
The traingle header file stores two vectors of vector 3s, one storing the three positions of the vertices making up the triangle, and the other storing the three normals of each vertex. It also stores a material for the triangle for findColor to use to color the geometry.
### Geometry
RTGeometry contains a list of elements representing all the triangles of the object, this is created through RTObj which uses a for loop to iterate through all positions and normals read in from the obj file and storing each three positions and normals as a triangle as described above in the Triangle header file.
### Scene
Scene stores all the values of the scene, including a vector of triangles which is the triangle soup by traversing over the scene graph and applies the view model matrix to the world coordinate system, and assigns the proper material for each triangle.
### Ray
The header file Ray contains the basepoint and direction. It has 5 functions in the cpp file RayTracer: 
- Raytrace(Camera cam, RTScene &scene, Image &image)
  - Ray trace calls RayThruPixel for all pixels, which returns a ray which is passed into intersect to find any hits the ray may encounter in the scene. Each hit is then passed to FindColor which finds the color of the pixel in the scene based on the material and the intersection position.
- RayThruPixel(cam, i, j, w, h)
  - Ray through pixel generates a ray originated from the camera position through the center of the (i,j) pixel into the world.
- Intersect(ray, scene)
  - Searches over all geometries in the scene and returns the closest hit.
- FindColor(hit)
  - Shades the light color seen by the incoming ray. Generates secondary rays to all lights, checking if it should be visible. A second mirror reflected ray is generated and the intersection between this new ray and the scene is calculated, adding it to the color of the pixel based on the specular multiplied by a recursive call of findColor of this new ray's intersection, subtracting one from the max recursion depth. When the max recursion depth is reached, the color is returned.
