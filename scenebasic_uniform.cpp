#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>

#include "helper/glutils.h"

#include "helper/texture.h"

using glm::vec3;
using glm::mat4;

float turnAxis = -300.0f;
int dir = 0;

//Relative position within world space
vec3 cameraPosition = vec3(0.0f, 5.0f, 3.0f);
//The direction of travel
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
//Up position within world space
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

SceneBasic_Uniform::SceneBasic_Uniform() : torus(0.7f, 0.3f, 30, 30),skyBox(100.0f), plane(50.0f,50.0f,1,1), tPrev(0.0f) {
}

void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);
    model = mat4(1.0f);
    view = glm::lookAt(vec3(0.5f, 0.75f, 0.75f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));
    projection = mat4(1.0f);

    prog.use();

    float x, z;
    for (int i = 0; i < 3; i++) {
        std::stringstream name;
        name << "lights[" << i << "].Position";
        x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
        z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
        prog.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z + 1.0f, 1.0f));
    }
    prog.setUniform("lights[0].L", vec3(0.0f, 0.0f, 0.8f));
    prog.setUniform("lights[1].L", vec3(0.0f, 0.8f, 0.0f));
    prog.setUniform("lights[2].L", vec3(0.8f, 0.0f, 0.0f));


    prog.setUniform("lights[0].La", vec3(0.0f, 0.0f, 0.2f));
    prog.setUniform("lights[1].La", vec3(0.0f, 0.2f, 0.0f));
    prog.setUniform("lights[2].La", vec3(0.2f, 0.0f, 0.0f));


    //skybox
    skyBoxProg.use();
    GLuint cubeTex = Texture::loadHdrCubeMap("media/texture/cube/pisa-hdr/pisa");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

    
    
}
void SceneBasic_Uniform::compile()
{
	try {
        skyBoxProg.compileShader("shader/skybox_frag.frag");
        skyBoxProg.compileShader("shader/skybox_vert.vert");
        
        skyBoxProg.link();
        skyBoxProg.use();

		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update(float t)
{
    float deltaT = t - tPrev;
    if (tPrev == 0.0f) {
        deltaT = 0.0f;
    }
    tPrev = t;
    

	//update your angle here
    if (m_animate) {
        turnAxis += 1.0f;
    }

}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    prog.use();

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -5.0f, -5.0f));
    model = glm::rotate(model, glm::radians(turnAxis), vec3(1.0f, 0.0f, 0.0f));

    prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ka", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ks", vec3(0.8f, 0.8f, 0.8f));
    prog.setUniform("Material.Shininess", 100.0f);

    setMatrices(prog);
    torus.render();//double check this
    
    model = mat4(1.0f);
    skyBoxProg.use();
    setMatrices(skyBoxProg);
    skyBox.render();
}


void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneBasic_Uniform::setMatrices(GLSLProgram& prog) {
    //Setting of matrices & related fog information
    glm::mat4 mv = view * model;

    prog.setUniform("modelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("projection", projection);
    prog.setUniform("ModelMatrix", model);

    //prog.setUniform("Fog.MinDistance", 1.f);
    //prog.setUniform("Fog.Color", vec3(0.5f, 0.5f, 0.5f));
}


void SceneBasic_Uniform::playerInput(float deltaTime, int dirT)
{
    dir = dirT;
    float movementSpeed = 0.3f * deltaTime;
    if (dir == 1) {//forwards
        cameraPosition += movementSpeed * cameraFront;
    }
    if (dir == 2) {//backwards
        cameraPosition -= movementSpeed * cameraFront;
    }
    if (dir == 3) {//left
        cameraPosition -= normalize(cross(cameraFront, cameraUp)) * movementSpeed;
    }
    if (dir == 4) {//right
        cameraPosition += normalize(cross(cameraFront, cameraUp)) * movementSpeed;
    }
    dir = 0;
}