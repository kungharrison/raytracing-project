class Image {
public:
	Image(int width, int height);
	void Initialize();
	std::vector<glm::vec3> setPixels(std::vector<glm::vec3> pixels);
private:
	int width;
	int height;
	std::vector<glm::vec3> pixels;
};
};