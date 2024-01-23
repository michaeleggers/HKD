
// HKD Game - 20.1.2024 - Michael Eggers


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <string>

#include <SDL.h>
#include <glad/glad.h>

#include "imgui.h"
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#define GLM_FORCE_RADIANS
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/ext.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "platform.h"
#include "iqm_loader.h"
#include "irender.h"
#include "r_gl.h"
#include "r_model.h"

#define PI                      3.14159265359
#define EPSILON                 0.00001



int main(int argc, char** argv)
{
    std::string exePath = hkd_GetExePath();

    IRender* renderer = new GLRender();
    if (!renderer->Init()) {
        SDL_Log("Could not initialize renderer.\n");
        return -1;
    }

    // Load IQM Model

    IQMModel iqmModel = LoadIQM((exePath + "../../assets/models/mrfixit/mrfixit.iqm").c_str());

    // Convert the model to our internal format

    HKD_Model model = CreateModelFromIQM(&iqmModel);

    // Upload this model to the GPU. This will add the model to the model-batch and you get an ID where to find the data in the batch?

    renderer->RegisterModel(&model);

    // Main loop
    
    bool shouldClose = false;
    float accumTime = 0.0f;    
    while (!shouldClose) {

        Uint32 startTime = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT) {
                shouldClose = true;
            }
        }

        renderer->RenderBegin();
        renderer->Render();
        renderer->RenderEnd();

        Uint32 endTime = SDL_GetTicks();
        Uint32 timePassed = endTime - startTime;
        float timePassedSeconds = (float)timePassed / 1000.0f;
        float FPS = 1.0f / timePassedSeconds;
        accumTime += timePassedSeconds;
        if (accumTime >= 1.0f) {
            //printf("FPS: %f\n", FPS);
            accumTime = 0.0f;
        }   
    }

    renderer->Shutdown();
    delete renderer;

    // Clean up
    SDL_Quit();

    return 0;
}
