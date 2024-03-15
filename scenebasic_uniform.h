#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "helper/torus.h"
#include "glm/glm.hpp"

#include "plane.h"
#include "skybox.h"

class SceneBasic_Uniform : public Scene
{
private:
    //GLuint vaoHandle; removed as it is handled by triangle mesh class (check lab 2)
    Torus torus;
    GLSLProgram prog;
    
    //float angle; becomes redundant (lab 2)

    //skybox variables
    SkyBox skyBox;
    Plane plane;

    GLSLProgram skyBoxProg;
    

    float tPrev;

    void setMatrices(GLSLProgram& prog);
    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
    void playerInput(float, int);
};

#endif // SCENEBASIC_UNIFORM_H
