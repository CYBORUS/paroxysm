#version 120
uniform samplerCube uTexture;

varying vec3 vTexture;

void main(void)
{
    gl_FragColor = textureCube(uTexture, vTexture);
}

