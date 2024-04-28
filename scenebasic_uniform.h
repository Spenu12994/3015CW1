#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "helper/torus.h"
#include "glm/glm.hpp"
#include "helper/objmesh.h"

#include "plane.h"
#include "skybox.h"

class SceneBasic_Uniform : public Scene
{
private:
    //GLuint vaoHandle; removed as it is handled by triangle mesh class (check lab 2)
    Torus torus;
    GLSLProgram prog;
    GLSLProgram toonProg;
    GLSLProgram spotlightProg;
    GLSLProgram edgeProg;
    GLSLProgram blurProg;
    GLSLProgram finalBlendProg;

    GLSLProgram lightBox;//code sampled from https://learnopengl.com/Advanced-Lighting/Bloom for testing purposes (bloom + hdr)

    GLuint fsQuad, fboHandle, renderTex;
    GLuint cement, moss;
    //float angle; becomes redundant (lab 2)

    //skybox variables
    SkyBox skyBox;
    Plane plane;

    GLSLProgram skyBoxProg;
    std::unique_ptr<ObjMesh> rollerCoaster;
    
    //spotlight
    float angle;

    float tPrev;

    void setMatrices(GLSLProgram& prog);
    void compile();
    void pass1();
    void pass2();
    void setupFBO();

public:
    SceneBasic_Uniform();
    void initScene();
    void update(float t);

    void render();
    void resize(int, int);
    void playerInput(float, int, int);
    void mouseInput(float, double, double);
};

#endif // SCENEBASIC_UNIFORM_H
