# 3015CW1
<h1>Comp 3015 course work 1 github page</h1>
<h2></h2>
youtube video: https://youtu.be/FUgGZeGzIzo<br>
Github: (https://github.com/Spenu12994/3015CW1/)<br>

<h2>What I started with</h2><br>
Starting with an adapted version of lab 1 (Lab Template), and scenebasic_uniform.cpp, .h, scene.h, scenerunner.h were all modified and expanded upon, along side various files being added to the helper folder from the labs

<h2>Application Contents</h2>
<h3>Theme</h3>
The theme for this project was to generate a rollercoaster/interactive experience, the end goal with cw2 is to set the player on a track and have them go through a rollercoaster tour of various shader techniques. this is why there are multiple shaders on serprate objects rather than just one centralized art style. the end goal is for an interactive museum of shaders.
<h3>Executable Usage</h3>

![image](https://github.com/Spenu12994/3015CW1/assets/91668500/bcaf1d7d-828e-4daa-9a44-8c7dc23e34b1)
<br>

The executable zip file comes as above, starting the app can simply be done by opening Project_Template.exe.
To generate a new random world, close and reopen the application.

<h3>Moving Pieces</h3>

<h4>main loop</h4>
Our main loop is within scenerunner.h and handles the calls to our scene to render and update. here in this mainloop we also get the user input and pass that data through to the scene. these inputs are mouse for looking and keyboard, using arrow keys and WASD for looking and movement respectively. We use a glfwSetCursorPosCallback(..); for our mouse inputs and simply a glfwgetkey check to get our keyboard inputs.

<h4>scenebasic_uniform.cpp</h4>
this class is where most of our application is ran. 

our constructor initialises our various models and times.
initScene() sets the default values for our MVP, loads and binds our textures and cubemap, sets our light information for our multilights and spotlights, and initialises our various shaders and GLPrograms

compile() compiles and readies our 4 different shaders ready for use.

update() gives us our delta time to calculate our animations and would be used for our mouse movements in a future implementation perhaps

render() where most of the seeable action happens. our render function places and loads our various models, and renders the new locations made from animations or lighting changes. most items are static, however our rollercoaster is transformed and rotated to our player movement so that it stays locked onto the bottom of the screen. the spotlight is also mapped to the downward face of our camera, lighting up our spotlight model when we are above it.

resize() is the standard resize method from lab template.

mouseInput() is our method which handles mouse inputs, taking delta time and the xpo and ypos from our scenerunner, updates our positions, makes sure we are within our limited pitch so we dont look too far up, then turns our new angle into a vec3 for our camerafront variable

playerInput() is called from scenerunner and handles our player input, using deltaTime and 2 integer values that tell us which keyboard directions is currently being pressed on WASD and the arrow keys, we simply move our camera based on which direction we are currently looking, movement is forward backward strafe left and strafe right on WASD, and rotate up rotate down, rotate left rotate right for our camera on arrowkeys.

<h4>Shaders</h4>
5 sets of vertex and fragment shaders for our various shading methods, however most derive from the phongmodel made in basic_uniform.

<h4>Basic_Uniform</h4>
basic_uniform frag and vert handle normal phong shading using textures and 3 lights of varying colours, the vert passes through our normal, position, and texture coordinates for our fragment shader. the fragmentshader normalizes our normal against our normal matrix, gets the coordinates of our camera/view by multiplaying our model view matrix against the position. 

wiht this information, we call our phong model 3 times, once for each light we have in the scene. here we combine our textures, and calculate our ambient, diffuse, and specular colours from our material details, finally multiplying them against our light colour and texture colour to recieve the final calculation, which is added to a running vec3 colour variable and then sent out through out FragColour variable

<h4>Basic_UniformSpotlight</h4>
this file is extremely similar to Basic_Uniform, with the main difference being our replacement of the lights structure with a spot light structure, which holds its radius and brightness and colour. in this fragment shader, we only need to run out phong model once, and here we run our normal colour calculations (without textures) while also making sure that the light is within our spotlight area of effect, finally multiplying the end result against our spotlight colour to give our stagelit render.

<h4>toonShader</h4>
this shader handles the stylized toony torus that can be seen. this is done by keeping almost all the same code from basic_uniform, however locking our colours at various intensities. by using our diffuse (line 36), we round our diffuse colours/dot product against a preset number of levels, making the lighting snap to colours rather than be one smooth gradient.

<h4>Basic_UniformFog</h4>
this file is the start of a fog shader, however was never fully implemented, and as such is not ever accessed within our program. this is kept here with the future hope of implementing fog.


<h4>skybox</h4>
this file is unique from the rest, using much less code. in our skybox shader vert, we simply pass forward our position and vertex position for the frag, however this position.xyz could be calculated in the fragment shader instead. in our fragment shader we simply get our skybox texture, calculate the position and colour, then pass it through FragColor to render our skybox.



<h3>Gameplay</h3>
The game is controlled with the following:<br>
WASD - Movement<br>
Mouse + ArrowKeys - Aim<br>
Esc - Quit<br>
<br>

<h3>Inspirations</h3>
as this application is not a final deployment, however the final design is somewhat inspired by themepark/carnival log ride shooters, or perhaps the slow cart rides at disney land that show off information before going into a park, the main example that comes to mind for me is a scene from American Arcadia, here is a timestamped video of a walkthrough to demonstrate what i mean: 
https://youtu.be/2DlqJz5L6l8?si=oL-ET2VdGxFrZfLE&t=3098
.<br>

resources used:
Comp 3015 lab
and lab resources
as well as stack overflow and learnopengl for a more indepth look into implementing features
