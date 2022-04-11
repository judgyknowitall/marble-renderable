# Marble Rendering

- *By: Zahra Ghavasieh*
- *Created in: April 2022*

This project aims to create a customizable marble texture in a real-time setting. It will make use of Perlin Noise to create a procedurally generated texture, the Schlick Fresnel Approximation to add reflections, and a subsurface scattering approximation technique to add more realism to this material.

---

## Project Plan

![Project-plan](https://github.com/judgyknowitall/marble-renderable/blob/main/docs/pics/plan.png)

During the month of April, this project was created for the CPSC591-Rendering course while completing my degree at the University of Calgary. 
This project consisted of five main stages:

1. Skeleton and Environment Setup
    - OpenGL window, shaders, buffers
    - Object Model Loading

2. Procedural Text Generation
    - Perlin Noise
    - Texture UV Mapping

3. Subsurface Scattering Approximation
    - Nvidia's Depth Map approach

4. Reflection Approximation
    - Schlick Fresnel Approximation

5. Tunings and Interactability
    - Changing Parameters 
    - Changing model

---

## Pipeline

![Project-pipeline](https://github.com/judgyknowitall/marble-renderable/blob/main/docs/pics/pipeline.png)

Using OpenGL, a deferred shading pipeline was implemented. Upon loading an object mesh, the marble texture is procedurally generated and stored in a vector.
Then, in the main loop, it is passed on to the Geometry shaders which will then create the fragment colours.
For the Subsurface Scattering (SSS) Approximation, the first pass will be to create the Depth map, which is also passed on to the Geometry shaders to simulate SSS.
Finally, all shading and reflections are done in the Lighting Pass. 

Therefore, this pipeline consists of three passes as shown above.

---

## Texture Generation

 *-- WIP --*


---

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/judgyknowitall/marble-renderable/settings/pages). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://docs.github.com/categories/github-pages-basics/) or [contact support](https://support.github.com/contact) and we’ll help you sort it out.
