#include <vector>
#include <glm/glm.hpp>

#ifndef __IMAGE_H__
#define __IMAGE_H__

class Image {
public:
	Image(int width, int height);
	void Initialize();
	void draw();
	std::vector<glm::vec3> setPixels(std::vector<glm::vec3> pixels);
private:
	int width;
	int height;
	unsigned int fbo;
	unsigned int texture;
	std::vector<glm::vec3> pixels;
};
};

#endif