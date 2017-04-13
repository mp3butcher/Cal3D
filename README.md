# Cal3D Official Git
Cal3D - 3d character animation library 

## Checkout
git clone --single-branch --recursive -b master https://github.com/mp3butcher/Cal3D.git cal3d

## Built
cd cal3d

mkdir built && cd built 

cmake ..
 

For Win32 users there a folder containing a version of glew and glut (cal3d\Win32GlutAndGlew)

## Main features
- skeleton based animation system with animation blending
- facial animation (throught morph target)
- examples viewers based on several rendering API 
  - viewer_opengl : OpenGL Software skinning
  - viewer_opengl_vp : OpenGL Hardware skinning
  - viewer_directx : DirectX Hardware skinning 
 
Because of API cleaning.. TiXmlBinding not working anymore (inner conventions are broken with return by ref).
It can be improve to comply with clean return by refs but it's not the mainly development focus
Tell us if you're using these (it's not a great deal to unclean)
