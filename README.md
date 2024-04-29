# 3015CW1

<h1>Image Processing Techniques</h1>

In order to use the following techniques, image processing techniques needed to be implemented.
to do this we must used our framerender buffer in order to get a capture of our screenspace render, which we can then manipulate and render to a quad the size of our screen.
Floating point framebuffers.
render quad:
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/e7d58f7a-99cc-4732-b0ca-41f6ab52afb1)

setting our frame buffer objects:
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/da890269-ff90-4d38-9bdc-494cbb159aa9)
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/eb9373c5-54cc-4d95-8724-d88b4575ecbc)

Bind our frame buffer before rendering:
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/e0ee1cf0-889b-44be-a4f6-87a773ea66ae)

rebind our frame buffer at the end to "capture" our rendering:
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/059f69c2-8e9e-440b-8bcd-18a3cbf520e6)

set our texture:
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/fce0c590-1b7a-4442-84b8-97aa3b5e241d)
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/cbeaf5e0-8d5b-43b9-adf0-3040600e7d1c)

<h2>HDR</h2> -
HDR allows us to render our lighting values past the max cap (1.0) and them tone them back down to bypass the lighting limits imposed by framebuffer caps on lighting.
we can use a floating point framebuffer (GL_RGBA16F), to bypass this limit.

now that our framebuffer can be specified as a floating point buffer, we can capture our scene data with 32 bits of color, allowing much brighter lights.
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/bb69771e-9d33-4bfa-b8bd-f4741020e62a)

![image](https://github.com/Spenu12994/3015CW1/assets/91668500/67049b6e-e159-45d9-a9eb-248e16d4a36a)

In our fragment shader, we can now render our scene with our new hdr values:
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/435554bd-a4f9-425e-8c92-8933884fd1e2)
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/768c1104-13c0-4d07-8cad-94dc60739eca)

vertex shader:
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/d090d2be-b081-466d-a30c-16a8abcb4d8a)

Final result:

Off:
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/0a173de7-5aa3-4dbb-b0fc-b7730e7d3e33)
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/ecc009cd-43fd-486e-bb5c-08fd985891f0)

On:
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/ff242b68-f1f8-403c-862e-d3ec677bbb9e)
![image](https://github.com/Spenu12994/3015CW1/assets/91668500/35761fcc-f0ab-45cd-a6d1-dc3d0413f4e0)

The richness of the colours are much clearer when HDR is on than off.

![image](https://github.com/Spenu12994/3015CW1/assets/91668500/9b8f5cd4-26ae-473e-af31-a068a298a0b5)


<h1>Keyboard Controls</h1>
F (5): HDR On
G: HDR Off
-
H: Bloom On
J: Bloom Off
-
C: Exposure Up
V: Exposure Down

