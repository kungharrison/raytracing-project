CC = g++ -no-pie
CFLAGS = -g -std=c++11 -Wno-deprecated-register -Wno-deprecated-declarations -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
INCFLAGS = -I./glm-0.9.7.1 -I./include -I/usr/include
LDFLAGS = -L./lib -L/usr/lib -lGLEW -lglut -lGLU -lGL -lX11 -lfreeimage

RM = /bin/rm -f
all: SceneViewer
SceneViewer: main.o Shader.o Camera.o RTObj.o RTScene.o Image.o RayTracer.o shaders/lighting.frag shaders/projective.vert
	$(CC) -o SceneViewer main.o Shader.o Camera.o RTObj.o RTScene.o Image.o RayTracer.o $(LDFLAGS)
main.o: src/main.cpp include/hw3AutoScreenshots.h include/Ray.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/main.cpp 
Shader.o: src/Shader.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Shader.cpp
Camera.o: src/Camera.cpp include/Camera.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Camera.cpp
RTObj.o: src/RTObj.cpp include/RTObj.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/RTObj.cpp
RTScene.o: src/RTScene.cpp src/RTScene.inl include/RTScene.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/RTScene.cpp
Image.o: src/Image.cpp include/Image.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Image.cpp
RayTracer.o: src/RayTracer.cpp include/Ray.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/RayTracer.cpp
clean: 
	$(RM) *.o SceneViewer

 
