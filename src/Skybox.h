#pragma once
#include <vector>
#include "SimpleGLShader.h"
#include "Camera.h"

class Skybox {
public:
	Skybox(std::string directory);
	~Skybox(void);
	void render(Camera* camera);
	GLuint cubemapTexture;
protected:
	GLuint skyboxVAO;
	GLuint skyboxVBO;
	SimpleGLShader ss;
	glm::mat4 model;

	GLuint loadCubemap(std::vector<std::string> faces);

};

