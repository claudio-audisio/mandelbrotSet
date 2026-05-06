#version 460
#extension GL_ARB_gpu_shader_fp64 : require

// Input from Raylib
in vec2 fragTexCoord;
in vec4 fragColor;

// Output
out vec4 finalColor;

// Uniforms passed from CPU
uniform dvec2 offset;
uniform double zoom;
uniform int maxIter;
uniform dvec2 resolution;

void main() {
    // Aspect ratio correction
    double aspect = resolution.x / resolution.y;
    dvec2 uv;
    uv.x = (double(gl_FragCoord.x) / resolution.x - 0.5) * aspect;
    uv.y = (double(gl_FragCoord.y) / resolution.y - 0.5);
    dvec2 c = uv / zoom + offset;

    dvec2 z = dvec2(0.0);
    int iter = maxIter;

    for (int i = 0; i < maxIter; i++) {
        if (z.x*z.x + z.y*z.y > 4.0) {
            iter = i;
            break;
        }
        z = dvec2(z.x*z.x - z.y*z.y + c.x, 2.0*z.x*z.y + c.y);
    }

    // Coloring
    if (iter == maxIter) {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);  // inside set = black
    } else {
        /**float t = mod(iter, 64) / float(64);
        finalColor = vec4(t / 2, t / 1.2, t * 1.2, 1);*/

        /**float t = float(iter) / float(64);
        float r = 0.5 + 0.5 * cos(6.28318 * t + 3.5);
        float g = 0.5 + 0.5 * cos(6.28318 * t + 3.8);
        float b = 0.5 + 0.5 * cos(6.28318 * t + 0.0);*/

        float t = float(iter) / float(64);
        float r = 0.5 + 0.5 * cos(6.28318 * t + 1.0);
        float g = 0.5 + 0.5 * cos(6.28318 * t + 0.5);
        float b = 0.5 + 0.5 * cos(6.28318 * t + 0.0);

        finalColor = vec4(r, g, b, 1.0);
    }
}