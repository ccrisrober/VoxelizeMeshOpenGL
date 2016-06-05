#ifndef _SIMPLE_SCENE_H
#define _SIMPLE_SCENE_H

#include "IScene.h"
#include "SimpleGLShader.h"
#include "objects/Mesh.h"
#include "Skybox.h"

#include "glm/glm.hpp"
#include <vector>

class SimpleScene : public IScene {
private:
	SimpleGLShader prog;

	RenderObject *cube;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

    void updateMatrices();
    void compileAndLinkShader();
    void drawScene();
	float angle = 0.0f;

	Skybox *skybox;

public:
	SimpleScene(int w, int h);
	~SimpleScene();

    void initScene();
    void update( float t );
	void draw(Camera* camera);
    void resize(int, int);

	float voxelSize = 0.5f;

};

#endif // _SIMPLE_SCENE_H
