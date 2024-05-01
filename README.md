# 3015CW1

<h1>Image Processing Techniques</h1>

In order to use the following techniques, image processing techniques needed to be implemented.
to do this we must used our framerender buffer in order to get a capture of our screenspace render, which we can then manipulate and render to a quad the size of our screen.
Floating point framebuffers.
render quad:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/e7d58f7a-99cc-4732-b0ca-41f6ab52afb1)

setting our frame buffer objects:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/da890269-ff90-4d38-9bdc-494cbb159aa9)<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/eb9373c5-54cc-4d95-8724-d88b4575ecbc)<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/8b265ab1-a706-4e5c-8ab8-bd24c9586f8b)<br>

Bind our frame buffer before rendering:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/e0ee1cf0-889b-44be-a4f6-87a773ea66ae)<br>

rebind our frame buffer at the end to "capture" our rendering:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/059f69c2-8e9e-440b-8bcd-18a3cbf520e6)<br>

set our texture:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/fce0c590-1b7a-4442-84b8-97aa3b5e241d)<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/cbeaf5e0-8d5b-43b9-adf0-3040600e7d1c)<br>

<h2>HDR</h2> -
HDR allows us to render our lighting values past the max cap (1.0) and them tone them back down to bypass the lighting limits imposed by framebuffer caps on lighting.
we can use a floating point framebuffer (GL_RGBA16F), to bypass this limit.

now that our framebuffer can be specified as a floating point buffer, we can capture our scene data with 32 bits of color, allowing much brighter lights.<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/bb69771e-9d33-4bfa-b8bd-f4741020e62a)<br>

![image](https://github.com/Spenu12994/3015CW1/assets/91668500/67049b6e-e159-45d9-a9eb-248e16d4a36a)<br>

In our fragment shader, we can now render our scene with our new hdr values:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/435554bd-a4f9-425e-8c92-8933884fd1e2)<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/768c1104-13c0-4d07-8cad-94dc60739eca)<br>

vertex shader:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/d090d2be-b081-466d-a30c-16a8abcb4d8a)<br>

Final result:<br>

Off:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/0a173de7-5aa3-4dbb-b0fc-b7730e7d3e33)<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/ecc009cd-43fd-486e-bb5c-08fd985891f0)<br>

On:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/ff242b68-f1f8-403c-862e-d3ec677bbb9e)<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/35761fcc-f0ab-45cd-a6d1-dc3d0413f4e0)<br>

The richness of the colours are much clearer when HDR is on than off.<br>

![image](https://github.com/Spenu12994/3015CW1/assets/91668500/9b8f5cd4-26ae-473e-af31-a068a298a0b5)<br>


<h2>Bloom</h2>

We manage bloom by getting an image of our scene, using a colour threshold to pick out the brightest colours, and then pushing the values through with a gaussian blur to create bloom.
<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/bb9ded99-9931-4331-b397-a45f88d153bb)<br>

![image](https://github.com/Spenu12994/3015CW1/assets/91668500/d3c5ac67-9cbf-4432-9f5f-abbb73448300)<br>

To achieve this, we must use 2 sets of floating point buffers, once for extracting the colours, and once for rendering the resulting scene.

Off:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/7ebe266d-1c48-4586-a699-623dcd97f43b)<br>

On:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/3e97fb26-bb49-440b-9690-805d993aefea)<br>



<h2>Gaussian Blur</h2>

Our Gaussian Blur implementation comes in the aid of our bloom, using gaussian blur and multiple passes, we can spread our the light on our object to push our bloom.<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/b4fc8739-e49e-4757-aab4-27065200d567)<br>
using a 2 pass rendering method, we can blur our scene multiple times to get the desired outcome:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/a84d9356-84d2-440a-823a-32fd6820d053)

High Blur:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/62bf1682-919b-4bec-af5a-3a4dc71c39a5)<br>

Low Blur:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/57dd8c6c-1e5f-4ade-b819-ff7349ed617f)<br>

Using a small change of code, we can extract our blur alone and see what it looks like:<br>

![image](https://github.com/Spenu12994/3015CW1/assets/91668500/a227a761-3951-42bf-9ac7-940c6473dfdd)<br>


<h1>Noise Effects</h1>
Our perlin noise texture is generated at our initScene() function, using the perlin noise generator class from the labs:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/6d994303-cfeb-42b3-92b2-4d65efffab7c)<br>
Our texture is saved to GL_TEXTURE2, as 0 holds our screenspace texture for 2 pass rendering, and slot 1 holds our bloom blur which is applied. A boolean "noise" is used to control whether or not the nightvision goggles are activated (discussed later).
In finalBlendShader and basic_uniform frag files, we access our noiseTexture as a uniform sampler2D, and use this texture to generate the following effects:<br>

<h2>Disintegration/Decay</h2><br>
In basicuniform.frag, we can use our noise texture, with an upper and lower boundary to select areas of our noise by sampling our alpha (noise.a) to pick out the black and white sections of the perlin noise:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/8cd59b1e-283f-478a-813a-a71b620f6cef)<br>
With this code, we can check if our noise is in the lower boundary ( a less than low ) or the upper boundary ( a greater than high), then check our disintegration boolean (disint) to decide if it should be disintegrated, if the boolean is true, we use the discard line to completely cancel the rendering process on this fragment, telling the shader to not render this fragment, giving us a disintegrated look:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/b68ac12e-76b5-4fdd-adcd-ed1a0f864ed1)<br>
this effect is applied onto our plane, a test torus for demonstration, and the cart you ride in, giving it a deteriorated and rustic look, complimented by the mixed mossy shaders.

<h2>Paint Splatter</h2><br>
The same as Disintegration, paint splatter uses the same higher or lower threshold given by our noise alpha to affect our render, however instead of termintating the render process, we instead will will in the gaps a completely different colour and shading:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/5f63f20c-b697-4df9-8ebc-1137be3a79b8)<br>
in this program I have used a matte blue to simulate a putty, however making it look like "paint" would be as easy as running it through the phong calculations, but adjusting the ambient, diffuse, and specular to give off a shinier and glossier finish.

<h2>Night Vision Goggles</h2>

The largest noise feature, Night Vision Goggles are actually created in our finalBlendShader.frag file, the shader used for our 2 stage rendering process, so that we can manipulate the final image of our render. The goggles apply a higher gamma threshold, allowing for much greater visibility, while also overlaying a green hue, as well as a noisy texture to simulate the static grime that comes with night vision goggles (at the very least, the night vision goggles that appear in modern games):<br>
![97DBC294-EE76-4290-8045-FA94FA4992FE_1_201_a-600x400](https://github.com/Spenu12994/3015CW1/assets/91668500/772f8bc8-0caa-4ecd-ba4d-b07927fef921)<br>

noiseBool is our boolean that controls whether our night vision goggles are equipped or not, and if true will rerun our tone mapping and gamma correcting, however this time with a much higher exposure, and by taking our final result and putting it through a luminance vector, mapping a green hue onto our scene:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/29e1f121-15e3-4917-bee1-85be9f296198)<br>

Here are the final results:<br>

Off:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/1816b705-62ee-4272-a3db-675a7f79e972)<br>
On:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/c77430cd-7577-440e-8ec4-d35bd093d281)<br>


Turning the exposure down, we can see just how much difference the goggles make:<br>

![image](https://github.com/Spenu12994/3015CW1/assets/91668500/f9e68c47-526f-4ee8-87af-8975483649ad)<br>

![image](https://github.com/Spenu12994/3015CW1/assets/91668500/0517e5c3-e175-4aec-a845-ce8c6f48b12b)<br>

allowing us to see features that could possibly be seen otherwise.

<h1>Lighting Techniques</h1>

Finally, from coursework 1, the spotlight feature has been fixed, updated, and adapted into a flashlight which follows the view and can be toggled on and off.<br>

This version now fully utilises our normalMatrix (in our vertex shader) to pass through our world location and normalize our normals, allowing our flashlight to full track and rotate onto our view.<br>

Another addition includes our distance and attenuation calculations (in our fragment shader), allowing for a more realistic spread of light, accurate to our flashlight:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/16313e13-d046-4aae-a8ee-2b3c3076e3dc)<br>

The final results look as follows:<br>
off:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/4a39ee15-85e4-41b6-a772-48894d6f2fc1)<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/be8778a8-2162-4dcb-bd0b-6f38dc369164)<br>

on:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/b28ecf46-bfb3-4b3d-ba7c-a6a65e76a2b8)<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/0a612efb-233f-4813-a8f0-3938afff9bd5)<br>






<h2></h2>

Additional:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/4894a4ad-90fd-4b92-bf1a-cec5c5620d93)<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/a411263d-6e06-4ba4-b6a6-c8e6ed49c946)<br>




<h1>Keyboard Controls</h1>
F (5): HDR On<br>
G: HDR Off<br>
-<br>
H: Bloom On<br>
J: Bloom Off<br>
-<br>
C: Exposure Up<br>
V: Exposure Down<br>
-<br>
B: Equip Night Vision Goggles<br>
N: Unequip Night Vision Goggles<br>
-<br>
Z: Disintegrate<br>
X: Fill in with putty<br>
-<br>
Left Click: Turn on Flashlight<br>
Right Click: Turn off Flashlight<br>


