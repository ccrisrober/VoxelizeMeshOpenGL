#include "Skybox.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "SOIL\SOIL.h"

#include <sstream>

Skybox::Skybox(std::string directory) {
	// Loads a cubemap texture from 6 individual texture faces
	// Order should be:
	// +X (right)
	// -X (left)
	// +Y (top)
	// -Y (bottom)
	// +Z (front)? (CHECK THIS)
	// -Z (back)?

	std::cout << "Load skybox ... ";
	std::vector<std::string> faces;
	faces.push_back(("../textures/" + directory + "/Right.jpg").c_str());
	faces.push_back(("../textures/" + directory + "/Left.jpg").c_str());
	faces.push_back(("../textures/" + directory + "/Top.jpg").c_str());
	faces.push_back(("../textures/" + directory + "/Bottom.jpg").c_str());
	faces.push_back(("../textures/" + directory + "/Back.jpg").c_str());
	faces.push_back(("../textures/" + directory + "/Front.jpg").c_str());

	std::stringstream vs;
	vs << "#version 430" << std::endl;
	vs << "layout (location = 0) in vec3 position;" << std::endl;
	vs << "out vec3 TexCoords;" << std::endl;
	vs << "uniform mat4 projection;" << std::endl;
	vs << "uniform mat4 view;" << std::endl;
	vs << "void main() {" << std::endl;
	vs << "	vec4 pos = projection * view * vec4(position, 1.0);" << std::endl;
	vs << "	gl_Position = pos.xyww;" << std::endl;
	vs << "	TexCoords = position;" << std::endl;
	vs << "}" << std::endl;

	std::stringstream fg;
	fg << "#version 430" << std::endl;
	fg << "in vec3 TexCoords;" << std::endl;
	fg << "out vec4 color;" << std::endl;
	fg << "uniform samplerCube skybox;" << std::endl;
	fg << "void main() { " << std::endl;
	fg << "	color = texture(skybox, TexCoords);" << std::endl;
	fg << "}" << std::endl;

	//ss.load("../shader/skybox.vert", "../shader/skybox.frag");
	ss.loadFromText(vs.str(), fg.str());
	ss.create(); ss.link();
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
  
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
  
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
   
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
  
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
  
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	// Setup skybox VAO
	glGenVertexArrays(1, &skyboxVAO); 
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
	cubemapTexture = loadCubemap(faces);  

	ss.add_uniform("view");
	ss.add_uniform("projection");

	std::cout << "done" << std::endl;

	faces.clear();
}

Skybox::~Skybox(void) {
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &skyboxVAO);
	//TODO: Y LAS TEXTURAS???!
}

GLuint Skybox::loadCubemap(std::vector<std::string> faces) {
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++) {
		image = SOIL_load_image(faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		free(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}
void Skybox::render(Camera* camera) {
	// Draw skybox as last
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
    ss.use();		
        
	glm::mat4 projection = camera->GetProjectionMatrix();
    glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));	// Remove any translation component of the view matrix
	ss.send_uniform("view", view);
	ss.send_uniform("projection", projection);

    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(ss.program(), "skybox"), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // Set depth function back to default	
	ss.unuse();
}
