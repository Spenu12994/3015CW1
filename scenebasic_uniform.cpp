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
#include "helper/noisetex.h"

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

bool torchOn = true;
bool disint = true;

GLuint cement;
GLuint moss;
GLuint screenSpace;


GLuint noiseTex;


struct LightInfo {
    glm::vec4 Position;
    vec3 La;
    vec3 L;
}lights[4];//multi light

SceneBasic_Uniform::SceneBasic_Uniform() : torus(0.7f, 0.3f, 30, 30), skyBox(100.0f), plane(25.0f,25.0f,1,1), tPrev(0.0f) {
    rollerCoaster = ObjMesh::load("media/rollercoaster/rollercoaster.obj", true, false), tunnel = ObjMesh::load("media/tunnel/untitled.obj", true, false);
}

void SceneBasic_Uniform::initScene()
{
    compile();

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    setupFBO();

    model = mat4(1.0f);
    view = glm::lookAt(vec3(0.5f, 0.75f, 0.75f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));
    projection = mat4(1.0f);

    prog.use();


    //load textures
    GLuint cubeTex = Texture::loadHdrCubeMap("media/texture/cube/pisa-hdr/pisa");

    cement = Texture::loadTexture("media/texture/cement.jpg");
    moss = Texture::loadTexture("media/texture/moss.png");
    

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, cement);
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, moss);

    

    float x, z;
    for (int i = 0; i < 3; i++) {
        std::stringstream name;
        name << "lights[" << i << "].Position";
        x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
        z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
        prog.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z + 1.0f, 1.0f));
        lights[i].Position = view * glm::vec4(x, 1.2f, z + 1.0f, 1.0f);
    }

    std::stringstream name;
    name << "lights[" << "3" << "].Position";
    x = 0.0f;
    z = 0.0f;
    prog.setUniform(name.str().c_str(), view * glm::vec4(x, 5.0f, z + 1.0f, 1.0f));
    prog.setUniform("lights[2].L", vec3(0.8f, 0.8f, 0.0f));
    prog.setUniform("lights[2].La", vec3(2.0f, 0.0f, 0.0f));

    prog.setUniform("disint", true);

    prog.setUniform("lights[0].L", vec3(0.0f, 0.0f, 0.8f));
    lights[0].L = vec3(0.0f, 0.0f, 0.8f);
    prog.setUniform("lights[1].L", vec3(0.0f,0.8f, 0.0f));
    lights[1].L = vec3(0.0f, 0.8f, 0.0f);
    prog.setUniform("lights[2].L", vec3(0.8f, 0.0f, 0.0f));
    lights[2].L = vec3(0.8f, 0.0f, 0.0f);

    prog.setUniform("lights[0].La", vec3(0.0f, 0.0f, 2.2f));
    prog.setUniform("lights[1].La", vec3(0.0f, 2.2f, 0.0f));
    prog.setUniform("lights[2].La", vec3(2.2f, 0.0f, 0.0f));
    lights[0].La = vec3(0.0f, 0.0f, 0.2f);
    lights[1].La = vec3(0.0f, 0.2f, 0.0f);
    lights[2].La = vec3(0.2f, 0.0f, 0.0f);

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

    /*
    //edge shader
    edgeProg.use();

    setupFBO();

    //fullscreen quad

    GLfloat verts[] = {
        -1.0f,-1.0f,0.0f,1.0f,-1.0f,0.0f,1.0f,1.0f,0.0f,
        -1.0f,-1.0f,0.0f,1.0f,1.0f,0.0f,-1.0f,1.0f,0.0f
    };

    GLfloat tc[] = {
        0.0f,0.0f,1.0f,0.0f,1.0f,1.0f,
        0.0f,0.0f,1.0f,1.0f,0.0f,1.0f
    };

    unsigned int handle[2];
    glGenBuffers(2, handle);
    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);


    //object setup

    glGenVertexArrays(1, &fsQuad);
    glBindVertexArray(fsQuad);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);//vertex pos

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);//texture coords
    glBindVertexArray(0);

    edgeProg.setUniform("EdgeThreshold", 0.05f);
    edgeProg.setUniform("light.L", vec3(1.0f));
    edgeProg.setUniform("light.La", vec3(0.2f));
    */

    



    spotlightProg.use();

    spotlightProg.setUniform("torchOn", torchOn);
    spotlightProg.setUniform("Spot.L", vec3(100.0f));
    spotlightProg.setUniform("Spot.La", vec3(0.5f));
    spotlightProg.setUniform("Spot.Exponent", 150.0f);
    spotlightProg.setUniform("Spot.Radius", glm::radians(20.0f));

    //skybox
    skyBoxProg.use();
    
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

    noiseTex = NoiseTex::generate2DTex(10.0f,0.5f,width/10,height/10);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE, noiseTex);


    blurProg.use();
    blurProg.setUniform("image", 0);
    finalBlendProg.use();
    finalBlendProg.setUniform("scene", 0);
    finalBlendProg.setUniform("bloomBlur", 1);
    finalBlendProg.setUniform("noiseTexture", 2);
    
}

unsigned int hdrFBO;
unsigned int colorBuffer;
unsigned int rboDepth;

unsigned int pingpongFBO[2];
unsigned int pingpongBuffer[2];
unsigned int colorBuffers[2];

float exposure = 0.20f;
bool bloom = true;
bool hdr = true;
bool noise = true;
unsigned int amount = 10;

void SceneBasic_Uniform::setupFBO() {

    // configure floating point framebuffer
    // ------------------------------------
    
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    // create floating point color buffer
   
    glGenTextures(2, colorBuffers);

    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
    }

    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    

    
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongBuffer);
    for (unsigned int i = 0; i < 2; i++){
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0);
        // also check if framebuffers are complete (no need for depth buffer)
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
    }

    /*
    //gen and bind framebuff


    glGenFramebuffers(1, &fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

    

    //create texture object
    glGenTextures(1, &renderTex);
    glBindTexture(GL_TEXTURE_2D, renderTex);

    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    //bind text to fbo
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);


    //create depth buf
    GLuint depthBuf;
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    //bind depth to fbo
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

    //set targets for fragment ourput var
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    //unbind framebuff and revert to default

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result == GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer is complete" << endl;
    }
    else {
        std::cout << "Framebuffer Error:" << result << endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    */
}


void SceneBasic_Uniform::compile()
{
	try {

        edgeProg.compileShader("shader/2PassRendering.vert");
        edgeProg.compileShader("shader/2PassRendering.frag");
        edgeProg.link();
        edgeProg.use();

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

        //https://learnopengl.com/Advanced-Lighting/Bloom
        blurProg.compileShader("shader/blurShader.vert");
        blurProg.compileShader("shader/blurShader.frag");
        blurProg.link();
        blurProg.use();

        finalBlendProg.compileShader("shader/finalBlendShader.vert");
        finalBlendProg.compileShader("shader/finalBlendShader.frag");
        finalBlendProg.link();
        finalBlendProg.use();

        lightBox.compileShader("shader/basic_uniform.vert");
        lightBox.compileShader("shader/light_box.frag");
        lightBox.link();
        lightBox.use();

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

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;

void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

/*
void SceneBasic_Uniform::pass1()
{
    edgeProg.setUniform("Pass", 1);

    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    glEnable(GL_DEPTH_TEST);


    edgeProg.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    edgeProg.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    edgeProg.setUniform("Material.Ka", vec3(0.2f, 0.55f, 0.9f));
    edgeProg.setUniform("Material.Ks", vec3(0.8f, 0.8f, 0.8f));
    edgeProg.setUniform("Material.Shininess", 100.0f);

    setMatrices(edgeProg);
    torus.render();
    
}
void SceneBasic_Uniform::pass2()
{
    edgeProg.setUniform("Pass", 2);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTex);

    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    model = mat4(1.0f);
    view = mat4(1.0f);
    projection = mat4(1.0f);

    setMatrices(edgeProg);

    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

}
*/

void SceneBasic_Uniform::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog.use();

    //set camera

    

    //render models

    //rollercoastd

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, cement);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, moss);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, noiseTex);
    

    view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);


    model = mat4(1.0f);
    model = glm::translate(model, cameraPosition);

    model = glm::rotate(model, glm::radians((-1*cameraYaw)-180), vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians((-1 * cameraPitch)), vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, vec3(-1.5f, -2.0f, 0.0f));

    prog.setUniform("disint", disint);

    prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ka", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ks", vec3(0.8f, 0.8f, 0.8f));
    prog.setUniform("Material.Shininess", 100.0f);

    setMatrices(prog);
    rollerCoaster->render();



    //regular plane
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -1.0f, 0.0f));

    prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ka", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ks", vec3(0.8f, 0.8f, 0.8f));
    prog.setUniform("Material.Shininess", 100.0f);

    setMatrices(prog);
    plane.render();

    //regular torus
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 0.0f, 0.0f));

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



    /*
    model = glm::rotate(model, glm::radians((-1 * cameraYaw) - 180), vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians((-1 * cameraPitch)), vec3(0.0f, 0.0f, 1.0f));
    */

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 0.0f, -2.0f));
    model = glm::rotate(model, glm::radians(turnAxis), vec3(1.0f, 0.0f, 0.0f));

    spotlightProg.setUniform("torchOn", torchOn);

    spotlightProg.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    spotlightProg.setUniform("Material.Ks", vec3(0.95f, 0.95f, 0.95f));
    spotlightProg.setUniform("Material.Ka", vec3(0.2f*0.3f,0.55f*0.3f,0.9f*0.3f));

    spotlightProg.setUniform("Spot.Position", cameraPosition);
    glm::mat3 normalMatrix = glm::mat3(glm::vec3(view[0]), glm::vec3(view[1]), glm::vec3(view[2]));
    spotlightProg.setUniform("Spot.Direction", cameraFront);

    
    spotlightProg.setUniform("Material.Shininess", 100.0f);

    setMatrices(spotlightProg);
    torus.render();

    //spotlight tunnel
    spotlightProg.use();

    
    spotlightProg.setUniform("Spot.Position", cameraPosition);
    normalMatrix = glm::mat3(glm::vec3(view[0]), glm::vec3(view[1]), glm::vec3(view[2]));
    spotlightProg.setUniform("Spot.Direction", cameraFront);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(-5.0f, 0.0f, -2.0f));

    spotlightProg.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    spotlightProg.setUniform("Material.Ks", vec3(0.95f, 0.95f, 0.95f));
    spotlightProg.setUniform("Material.Ka", vec3(0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f));

    spotlightProg.setUniform("Material.Shininess", 100.0f);

    setMatrices(spotlightProg);
    tunnel->render();
  


    //skybox
    model = mat4(1.0f);
    skyBoxProg.use();
    setMatrices(skyBoxProg);
    skyBox.render();


    //light box

    lightBox.use();
    

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.0f,-3.5f));
    model = glm::scale(model, glm::vec3(1.0f));

    setMatrices(lightBox);
    lightBox.setUniform("lightColor", vec3(5.0f, 5.0f, 5.0f));
    torus.render();

    /*
    //visualise lights
    for (int i = 0; i < 3; i++) {
        model = glm::mat4(1.0f);
        vec3 v3 = vec3(lights[i].Position.x, lights[i].Position.y, lights[i].Position.z);
        model = glm::translate(model, v3);
        model = glm::scale(model, vec3(1.2f));

        
        lightBox.setUniform("lightColor",lights[i].La);
        setMatrices(lightBox);
        torus.render();
    }
    */


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 2. blur bright fragments with two-pass Gaussian Blur 
// --------------------------------------------------
    bool horizontal = true, first_iteration = true;
    
    blurProg.use();

    for (unsigned int i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
        blurProg.setUniform("horizontal", horizontal);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongBuffer[!horizontal]);
        renderQuad();
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    finalBlendProg.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, noiseTex);

    finalBlendProg.setUniform("bloom", bloom);
    finalBlendProg.setUniform("hdr", hdr);
    finalBlendProg.setUniform("noiseBool", noise);
    finalBlendProg.setUniform("exposure", exposure);
    setMatrices(finalBlendProg);
    renderQuad();

    
    /*
    pass1();
    glFlush();
    pass2();
    */
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

void SceneBasic_Uniform::playerInput(float deltaTime, int dirT, int angT, int clickPress)
{
    dir = dirT;
    float movementSpeed = 0.002f * deltaTime;

    switch (dir) {
    case 1:
        cameraPosition += movementSpeed * cameraFront;
        break;
    case 2:
        cameraPosition -= movementSpeed * cameraFront;
        break;
    case 3:
        cameraPosition -= normalize(cross(cameraFront, cameraUp)) * movementSpeed;
        break;
    case 4:
        cameraPosition += normalize(cross(cameraFront, cameraUp)) * movementSpeed;
        break;
    case 5:
        hdr = true;
        break;
    case 6:
        hdr = false;
        break;
    case 7:
        bloom = true;
        break;
    case 8:
        bloom = false;
        break;
    case 9:
            if (exposure < 100)
                exposure += 0.01f;

        break;
    case 10:
        if (exposure > 0)
            exposure -= 0.01f;
        break;
    case 11:
        noise = true;
        break;
    case 12:
        noise = false;
        break;
    case 13:
        disint = true;
        break;
    case 14:
        disint = false;
        break;
    default:

        break;
    }
    /*
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
    if (dir == 5) {
        hdr = true;
    }
    if (dir == 6) {
        hdr = false;
    }
    if (dir == 7) {
        bloom = true;
    }
    if (dir == 8) {
        bloom = false;
    }
    if (dir == 9) {
        if(exposure < 100)
        exposure += 0.01f;
    }
    if (dir == 10) {
        if (exposure > 0)
        exposure -= 0.01f;
    }
    if (dir == 11) {
        noise = true;
    }
    if (dir == 12) {
        //reserved
        noise = false;
    }
    if (dir == 13) {
        //reserved
    }
    if (dir == 14) {
        
    }
    */

    dir = 0;

    switch (clickPress) {
    case 1:
        torchOn = true;
        break;
    case 2:
        torchOn = false;
        break;
    default:
        
        break;
    }

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