#include "game.h"
#include "r_itexture.h"

#include "imgui.h"

static int hkd_Clamp(int val, int clamp) {
    if (val > clamp || val < clamp) return clamp;
    return val;
}

Game::Game(std::string exePath, IRender* renderer)
{
    m_Renderer = renderer;
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


    //int hRenderModel3 = m_Renderer->RegisterModel(&m_Model);

    //Entity player = {};
    //player.SetModel(model);
}

bool Game::RunFrame(double dt)
{
    // Update game state    

    for (auto& model : m_Models) {
        UpdateModel(model, (float)dt);
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

    
    //for (int i = 0; i < showModelInspector.size(); i++) {
    //    if (showModelInspector[i]) {
    //        ITexture* texture = textures[i];
    //        char buffer[256];
    //        sprintf(buffer, "%s, %d x %d", texture->m_Filename.c_str(), texture->m_Width, texture->m_Height);
    //        ImGui::Begin(buffer);

    //        ImGui::Image(
    //            (void*)(texture->m_hGPU),
    //            ImVec2(texture->m_Width, texture->m_Height)
    //        );

    //        ImGui::End();
    //    }
    //}

    m_Renderer->Render(m_Models);

    m_Renderer->RenderEnd();

    return true;
}

void Game::Shutdown()
{
    m_Renderer->Shutdown();
    delete m_Renderer;
}
