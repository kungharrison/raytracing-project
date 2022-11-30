#include "Image.h"
#include <GL/glew.h>
#include <GL/glut.h>

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