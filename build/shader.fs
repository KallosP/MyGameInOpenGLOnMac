#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D material; // 0
//uniform sampler2D mask; // 1
//uniform bool useMask;

void main()
{
    vec4 color = texture(material, TexCoord);

    //if (useMask) {
        //vec4 maskColor = texture(mask, TexCoord);
        //// Mix material and mask with weight 0.2
        //color = mix(color, maskColor, 0.2);
    //}

    FragColor = color;
}