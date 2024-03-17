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

//Camera sidways rotation
float cameraYaw = -90.0f;
//Camera vertical rotation
float cameraPitch = 0.0f;
//Determines if first entry of mouse into window
bool mouseFirstEntry = true;
//Positions of camera from given last frame
float cameraLastXPos = 800.0f / 2.0f;
float cameraLastYPos = 600.0f / 2.0f;

SceneBasic_Uniform::SceneBasic_Uniform() : torus(0.7f, 0.3f, 30, 30), skyBox(100.0f), plane(25.0f,25.0f,1,1), tPrev(0.0f) {
    rollerCoaster = ObjMesh::load("media/rollercoaster/rollercoaster.obj", true, false);
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

    GLuint texID = Texture::loadTexture("media/texture/brick1.jpg");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);

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

    toonProg.use();

    for (int i = 0; i < 3; i++) {
        std::stringstream name;
        name << "lights[" << i << "].Position";
        x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
        z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
        toonProg.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z + 1.0f, 1.0f));
    }
    toonProg.setUniform("lights[0].L", vec3(0.0f, 0.0f, 0.8f));
    toonProg.setUniform("lights[1].L", vec3(0.0f, 0.8f, 0.0f));
    toonProg.setUniform("lights[2].L", vec3(0.8f, 0.0f, 0.0f));


    toonProg.setUniform("lights[0].La", vec3(0.0f, 0.0f, 0.2f));
    toonProg.setUniform("lights[1].La", vec3(0.0f, 0.2f, 0.0f));
    toonProg.setUniform("lights[2].La", vec3(0.2f, 0.0f, 0.0f));

    spotlightProg.use();
    spotlightProg.setUniform("Spot.L", vec3(0.9f));
    spotlightProg.setUniform("Spot.La", vec3(0.5f));
    spotlightProg.setUniform("Spot.Exponent", 150.0f);
    spotlightProg.setUniform("Spot.Radius", glm::radians(15.0f));


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


        toonProg.compileShader("shader/toonShader.vert");
        toonProg.compileShader("shader/toonShader.frag");
        toonProg.link();
        toonProg.use();

        spotlightProg.compileShader("shader/basic_uniformspotlight.vert");
        spotlightProg.compileShader("shader/basic_uniformspotlight.frag");
        spotlightProg.link();
        spotlightProg.use();

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
    prog.use();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    //set camera

    

    //render models

    //rollercoastd

    view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

    prog.use();

    model = mat4(1.0f);
    model = glm::translate(model, cameraPosition);

    model = glm::rotate(model, glm::radians((-1*cameraYaw)-180), vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians((-1 * cameraPitch)), vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, vec3(-1.5f, -2.0f, 0.0f));

    prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ka", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ks", vec3(0.8f, 0.8f, 0.8f));
    prog.setUniform("Material.Shininess", 100.0f);

    setMatrices(prog);
    rollerCoaster->render();




    //regular plane
    prog.use();
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -1.0f, 0.0f));

    prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ka", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ks", vec3(0.8f, 0.8f, 0.8f));
    prog.setUniform("Material.Shininess", 100.0f);

    setMatrices(prog);
    plane.render();

    //regular torus
    prog.use();

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 0.0f, -2.0f));
    model = glm::rotate(model, glm::radians(turnAxis), vec3(1.0f, 0.0f, 0.0f));

    prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ka", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ks", vec3(0.8f, 0.8f, 0.8f));
    prog.setUniform("Material.Shininess", 100.0f);

    setMatrices(prog);
    torus.render();

    //toon shaded
    toonProg.use();
    

    model = mat4(1.0f);
    model = glm::translate(model, vec3(-2.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(turnAxis), vec3(1.0f, 0.0f, 0.0f));

    toonProg.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    toonProg.setUniform("Material.Ka", vec3(0.2f, 0.55f, 0.9f));
    toonProg.setUniform("Material.Ks", vec3(0.8f, 0.8f, 0.8f));
    toonProg.setUniform("Material.Shininess", 100.0f);

    setMatrices(toonProg);
    torus.render();


    //spotlight torus
    spotlightProg.use();

    glm::vec4 lightPos = glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);
    spotlightProg.setUniform("Spot.Position", (view * lightPos));
    glm::mat3 normalMatrix = glm::mat3(glm::vec3(view[0]), glm::vec3(view[1]), glm::vec3(view[2]));
    spotlightProg.setUniform("Spot.Direction", normalMatrix * glm::vec3(-lightPos));

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(turnAxis), vec3(1.0f, 0.0f, 0.0f));

    spotlightProg.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    spotlightProg.setUniform("Material.Ks", vec3(0.95f, 0.95f, 0.95f));
    spotlightProg.setUniform("Material.Ka", vec3(0.2f*0.3f,0.55f*0.3f,0.9f*0.3f));
    
    spotlightProg.setUniform("Material.Shininess", 100.0f);

    setMatrices(spotlightProg);
    torus.render();


    //skybox
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
    //set matrices
    glm::mat4 mv = view * model;//model view

    prog.setUniform("modelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("projection", projection);
    prog.setUniform("ModelMatrix", model);

}

void SceneBasic_Uniform::mouseInput(float deltaTime, double xpos, double ypos)
{
    //Initially no last positions, so sets last positions to current positions
    if (mouseFirstEntry)
    {
        cameraLastXPos = (float)xpos;
        cameraLastYPos = (float)ypos;
        mouseFirstEntry = false;
    }

    //Sets values for change in position since last frame to current frame
    float xOffset = (float)xpos - cameraLastXPos;
    float yOffset = cameraLastYPos - (float)ypos;

    //Sets last positions to current positions for next frame
    cameraLastXPos = (float)xpos;
    cameraLastYPos = (float)ypos;

    //Moderates the change in position based on sensitivity value
    const float sensitivity = 0.025f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    //Adjusts yaw & pitch values against changes in positions
    cameraYaw += xOffset;
    cameraPitch += yOffset;

    //Prevents turning up & down beyond 90 degrees to look backwards
    if (cameraPitch > 89.0f)
    {
        cameraPitch = 89.0f;
    }
    else if (cameraPitch < -89.0f)
    {
        cameraPitch = -89.0f;
    }

    //Modification of direction vector based on mouse turning
    vec3 direction;
    direction.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    direction.y = sin(glm::radians(cameraPitch));
    direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = normalize(direction);
}

void SceneBasic_Uniform::playerInput(float deltaTime, int dirT, int angT)
{
    dir = dirT;
    float movementSpeed = 0.002f * deltaTime;
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


    float lookOffset = 0.4f;

    switch (angT) {
        case 1: {
            cameraPitch += lookOffset;
            break;
        }
        case 2: {
            cameraPitch -= lookOffset;
            break;
        }
        case 3: {
            cameraYaw -= lookOffset;
            break;
        }
        case 4: {
            cameraYaw += lookOffset;
            break;
        }
    }
    

    //Prevents turning up & down beyond 90 degrees to look backwards
    if (cameraPitch > 89.0f)
    {
        cameraPitch = 89.0f;
    }
    else if (cameraPitch < -89.0f)
    {
        cameraPitch = -89.0f;
    }

    //Modification of direction vector based on mouse turning
    vec3 direction;
    direction.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    direction.y = sin(glm::radians(cameraPitch));
    direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = normalize(direction);


}