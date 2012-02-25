#version 120
uniform mat4 uMVPM;
uniform vec3 uAcceleration;
uniform float uTime;

attribute vec3 iPosition;
attribute vec3 iVelocity;
attribute float iDelay;
attribute vec4 iColor;

varying vec4 vColor;
const vec4 HiddenColor = vec4(0.0, 0.0, 0.0, 0.0)

void main(void)
{
    float t = uTime - iDelay;
    if (t >= 0.0)
    {
        vec3 finalPosition = iPosition + iVelocity * uTime
            + 0.5 * uAcceleration * uTime * uTime;
    
        vec4 p = vec4(finalPosition, 1.0);
    
        vColor = iColor;
        gl_Position = uMVPM * p;
    }
    else
    {
        vColor = HiddenColor;
        gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    }
}

