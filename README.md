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

Additional:<br>
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/4894a4ad-90fd-4b92-bf1a-cec5c5620d93)<br>




<h1>Keyboard Controls</h1>
F (5): HDR On
G: HDR Off
-
H: Bloom On
J: Bloom Off
-
C: Exposure Up
V: Exposure Down

