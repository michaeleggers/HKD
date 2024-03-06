#version 330

in vec2 TexCoord;
in vec3 BaryCentricCoords;
in vec3 Normal;
in vec4 Color;

out vec4 out_Color;

layout (std140) uniform Settings {
    uint drawWireframe;
};

float edgeFactor(){
    vec3 d = fwidth(BaryCentricCoords);
    vec3 a3 = smoothstep(vec3(0.0), d, BaryCentricCoords);
    return min(min(a3.x, a3.y), a3.z);
}

void main() {

    vec4 wireframe = vec4(0.0);
    if (drawWireframe == 1U) {
        wireframe = vec4(mix(vec3(1.0), vec3(0.0), edgeFactor()), 1.0);
        wireframe.a = 0.2;
        wireframe.rgb *= wireframe.a;
    }

    out_Color = vec4(Color.rgb + wireframe.rgb, 1.0);  
}
