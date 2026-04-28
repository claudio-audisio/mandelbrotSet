#version 330

// Input from Raylib
in vec2 fragTexCoord;
in vec4 fragColor;

// Output
out vec4 finalColor;

// Uniforms passed from CPU
uniform vec2 offset;
uniform float zoom;
uniform int maxIter;
uniform vec2 resolution;

void main() {
    // Aspect ratio correction
    float aspect = resolution.x / resolution.y;
    vec2 uv;
    uv.x = (gl_FragCoord.x / resolution.x - 0.5) * aspect;
    uv.y = (gl_FragCoord.y / resolution.y - 0.5);
    vec2 c = uv / zoom + offset;

    vec2 z = vec2(0.0);
    int iter = maxIter;

    for (int i = 0; i < maxIter; i++) {
        if (dot(z, z) > 4.0) {
            iter = i;
            break;
        }
        z = vec2(z.x*z.x - z.y*z.y + c.x, 2.0*z.x*z.y + c.y);
    }

    // Coloring
    if (iter == maxIter) {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);  // inside set = black
    } else {
        float t = float(iter) / float(256);
        finalColor = vec4(t, t * 2, t * 3, 1);
    }
}