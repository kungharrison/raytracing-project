#include "Image.h"
#include <GL/glew.h>
#include <GL/glut.h>

Image::Image(int width, int height) {
	this->width = width;
	this->height = height;
}

void Image::initialize() {
	pixels.reserve(width * height);
	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &texture);
}

// This function is to test the file
void Image::setPixels() {
	for (int c = 0; c < width; c++) {
		for (int r = 0; r < height; r++) {
			pixels[r + height * c] = glm::vec3(0.6, 0.2, 0.5);
		}
	}
}

void Image::draw(void) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, // load texture
		0, GL_RGB, GL_FLOAT, &pixels[0][0]);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, texture, 0);// attach texture and the read frame
	//glBindFramebuffer(GL_WRITE_FRAMEBUFFER, 0); // if not already so
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT,
		GL_NEAREST); // copy framebuffer from read to write
}