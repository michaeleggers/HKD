#include "game.h"

#include <SDL.h>

#include "r_itexture.h"
#include "camera.h"
#include "input.h" 

#include "imgui.h"

static int hkd_Clamp(int val, int clamp) {
    if (val > clamp || val < clamp) return clamp;
    return val;
}

Game::Game(std::string exePath, hkdInterface* interface, IRender* renderer)
{
    m_Renderer = renderer;
    m_Interface = interface;
    m_ExePath = exePath;
}

void Game::Init()
{
    // Load IQM Model

    //IQMModel iqmModel  = LoadIQM((m_ExePath + "../../assets/models/cylinder_two_anims/cylinder_two_anims.iqm").c_str());
    IQMModel iqmModel = LoadIQM((m_ExePath + "../../assets/models/mrfixit/mrfixit.iqm").c_str());
    IQMModel iqmModel2 = LoadIQM((m_ExePath + "../../assets/models/cylinder_two_anims/cylinder_two_anims.iqm").c_str());
    IQMModel iqmModel3 = LoadIQM((m_ExePath + "../../assets/models/hana/hana.iqm").c_str());

    // Convert the model to our internal format

    m_Model = CreateModelFromIQM(&iqmModel);
    m_Model2 = CreateModelFromIQM(&iqmModel2);
    m_Model3 = CreateModelFromIQM(&iqmModel3);

    m_Models.push_back(&m_Model);
    m_Models.push_back(&m_Model2);
    m_Models.push_back(&m_Model3);

    // Upload this model to the GPU. This will add the model to the model-batch and you get an ID where to find the data in the batch?

    int hRenderModel = m_Renderer->RegisterModel(&m_Model);
    int hRenderModel2 = m_Renderer->RegisterModel(&m_Model2);
    int hRenderModel3 = m_Renderer->RegisterModel(&m_Model3);

    // Camera

    m_Camera = Camera(glm::vec3(0, -5, 8.0));
}

bool Game::RunFrame(double dt)
{
    // Update game state    

    for (auto& model : m_Models) {
        UpdateModel(model, (float)dt);
    }

    // Test Mouse input

    if (MouseWentDown(SDL_BUTTON_LEFT)) {
        printf("Left mouse button went down.\n");
    }
    if (MouseWentUp(SDL_BUTTON_LEFT)) {
        printf("Left mouse button went up.\n");
    }

    if (MouseWentDown(SDL_BUTTON_RIGHT)) {
        printf("Right mouse button went down.\n");
    }
    if (MouseWentUp(SDL_BUTTON_RIGHT)) {
        printf("Right mouse button went up.\n");
    }

    if (MousePressed(SDL_BUTTON_LEFT)) {
        printf("Left mouse button pressed\n");
    }
    if (MousePressed(SDL_BUTTON_RIGHT)) {
        printf("Right mouse button pressed\n");
    }
    // Want to quit on ESCAPE

    if (KeyPressed(SDLK_ESCAPE)) {
        m_Interface->QuitGame();
    }

    // Update camera

    if (KeyPressed(SDLK_w)) {
        m_Camera.Pan((float)dt * 0.5f * m_Camera.m_Forward);
    }
    if (KeyPressed(SDLK_s)) {
        m_Camera.Pan((float)dt * 0.5f * -m_Camera.m_Forward);
    }
    if (KeyPressed(SDLK_d)) {
        m_Camera.Pan((float)dt * 0.5f * m_Camera.m_Side);
    }
    if (KeyPressed(SDLK_a)) {
        m_Camera.Pan((float)dt * 0.5f * -m_Camera.m_Side);
    }

    if (KeyPressed(SDLK_RIGHT)) {
        m_Camera.RotateAroundUp(-0.2f * dt);
    }
    if (KeyPressed(SDLK_LEFT)) {
        m_Camera.RotateAroundUp(0.2f * dt);
    }

    if (KeyPressed(SDLK_UP)) {
        m_Camera.RotateAroundSide(0.2f * dt);
    }
    if (KeyPressed(SDLK_DOWN)) {
        m_Camera.RotateAroundSide(-0.2f * dt);
    }
    // Render stuff

    m_Renderer->RenderBegin();

    ImGui::ShowDemoWindow();

    static std::vector<bool> showModelInspector;
    std::vector<ITexture*> textures = m_Renderer->Textures();
    showModelInspector.resize(textures.size());

    int textureID = 0;   

    // Display textures

    ImGui::Begin("Textures");

    for (auto& texture : textures) {

        int width = texture->m_Width;
        int height = texture->m_Height;

        ImGui::Text("%s, %d x %d", texture->m_Filename.c_str(), width, height);

        if (
            ImGui::ImageButton(
                (void*)(texture->m_hGPU),
                ImVec2(
                    hkd_Clamp(width, 128),
                    hkd_Clamp(height, 128))
            )
            )
        {
            showModelInspector[textureID] = !showModelInspector[textureID];
        }

        textureID++;
    }

    ImGui::End();   
    
    for (int i = 0; i < showModelInspector.size(); i++) {
        if (showModelInspector[i]) {
            ITexture* texture = textures[i];
            char buffer[256];
            sprintf(buffer, "%s, %d x %d", texture->m_Filename.c_str(), texture->m_Width, texture->m_Height);
            ImGui::Begin(buffer);

            ImGui::Image(
                (void*)(texture->m_hGPU),
                ImVec2(texture->m_Width, texture->m_Height)
            );

            ImGui::End();
        }
    }

    m_Renderer->Render(&m_Camera, m_Models);

    m_Renderer->RenderEnd();

    return true;
}

void Game::Shutdown()
{
    m_Renderer->Shutdown();
    delete m_Renderer;
}
