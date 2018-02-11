#version 150

in  vec2 vPosition;
in  vec3 vColor;
out vec4 color;

void
main()
{
    gl_Position = vec4(vPosition, 0.5, 1);
    color = vec4(vColor, 1.0);
}
