#include "SimpleScene.h"
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/constants.hpp"

SimpleScene::SimpleScene(int w, int h) : IScene(w, h) { }
SimpleScene::~SimpleScene() {
}

void SimpleScene::initScene() {
	compileAndLinkShader();
	glClearColor(0.5f,0.5f,0.5f,1.0f);

	cube = new Mesh("../models/GreatJaggi.obj");
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	skybox = new Skybox("skybox");
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


void SimpleScene::update( float t ) {
	if (animating())
		angle += 0.1 * t;
}

void SimpleScene::draw(Camera* camera) {
	prog.use();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view = camera->GetViewMatrix();
	projection = camera->GetProjectionMatrix();

	prog.send_uniform("lightPosition", glm::vec3(view * vec4(1.0, 1.0, 0.0, 1.0)));
	prog.send_uniform("viewPos", camera->Position);

	// draw scene
	drawScene();

	prog.unuse();

	skybox->render(camera);
}

void SimpleScene::updateMatrices() {
	glm::mat4 modelView = view * model;

	prog.send_uniform("normal", glm::mat3(glm::vec3(modelView[0]), glm::vec3(modelView[1]), glm::vec3(modelView[2])));
	prog.send_uniform("modelView", modelView);
	prog.send_uniform("modelViewProj", projection * modelView);
}

void SimpleScene::resize(int w, int h) {
	glViewport(0, 0, w, h);
	width = w;
	height = h;
}

void SimpleScene::compileAndLinkShader() {
	prog.load("../shader/voxel.vert", GL_VERTEX_SHADER);
	prog.load("../shader/voxel.frag", GL_FRAGMENT_SHADER);
	prog.load("../shader/voxel.geom", GL_GEOMETRY_SHADER);
	prog.compile_and_link();
	prog.add_uniform("normal");
	prog.add_uniform("modelView");
	prog.add_uniform("modelViewProj");
	prog.add_uniform("lightPosition");
	prog.add_uniform("viewPos");
	prog.add_uniform("skybox");
	prog.add_uniform("voxelSize");
	//prog.use();
	//glUniform1i(prog.uniform("skybox"), 0);
	//prog.unuse();
}

void SimpleScene::drawScene() {
	int n = 0;
	float size = 0.5f;
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubemapTexture);
	prog.send_uniform("voxelSize", voxelSize);
	model = glm::translate(mat4(1.0f), vec3(0.0));
	//model = glm::rotate(model, glm::radians(-angle), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-angle), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(size));
	updateMatrices();
	cube->render();
}
