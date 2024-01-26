#version 330


in vec2 TexCoord;
in vec3 BaryCentricCoords;
in vec3 Normal;

out vec4 out_Color;

// layout (std140, binding = 1) uniform PerFrameSettings {
//     uint drawWireframe; // TODO: Check how to reliably pass an actual bool from CPU to shader.
// };

uniform sampler2D colorTex;

float lookUp[9] = float[9](
    0,
    32,
    32*2,
    32*3,
    32*4,
    32*5,
    32*6,
    32*7,
    32*8
);


float edgeFactor(){
    vec3 d = fwidth(BaryCentricCoords);
    vec3 a3 = smoothstep(vec3(0.0), d, BaryCentricCoords);
    return min(min(a3.x, a3.y), a3.z);
}

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main() {

    vec4 texColor = texture(colorTex, TexCoord);

    uint idxR = uint(map(texColor.r, 0.0, 1.0, 0.0, 8.0));
    uint idxG = uint(map(texColor.g, 0.0, 1.0, 0.0, 8.0));
    uint idxB = uint(map(texColor.b, 0.0, 1.0, 0.0, 8.0));

    vec4 color;
    color.r = float(lookUp[idxR]) / 255.0f,
    color.g = float(lookUp[idxG]) / 255.0f;
    color.b = float(lookUp[idxB]) / 255.0f;
    color.a = 1.0f;
    
    // vec4 wireframe = vec4(0.0);
    // if (drawWireframe == 1) {
    //     wireframe = vec4(mix(vec3(1.0), vec3(0.0), edgeFactor()), 1.0);        
    // }

    vec4 wireframe = vec4(mix(vec3(1.0), vec3(0.0), edgeFactor()), 1.0);
    wireframe.a = 0.2;
    wireframe.rgb *= wireframe.a;
    
    vec3 normalColor = 0.5*Normal + 0.5;

    out_Color = vec4(color.rgb + wireframe.rgb, 1.0);
    //out_Color = vec4(normalColor.rgb, 1.0);

}