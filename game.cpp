#include "game.h"
#include "r_itexture.h"

#include "imgui.h"

Game::Game(std::string exePath, IRender* renderer)
{
    m_Renderer = renderer;
    m_ExePath = exePath;
}

void Game::Init()
{
    // Load IQM Model

    IQMModel iqmModel  = LoadIQM((m_ExePath + "../../assets/models/mrfixit/mrfixit.iqm").c_str());
    IQMModel iqmModel2 = LoadIQM((m_ExePath + "../../assets/models/hana/hana.iqm").c_str());

    // Convert the model to our internal format

    m_Model = CreateModelFromIQM(&iqmModel);
    HKD_Model model2 = CreateModelFromIQM(&iqmModel2);

    // Upload this model to the GPU. This will add the model to the model-batch and you get an ID where to find the data in the batch?

    int hRenderModel = m_Renderer->RegisterModel(&m_Model);
    int hRenderModel2 = m_Renderer->RegisterModel(&model2);

    int hRenderModel3 = m_Renderer->RegisterModel(&m_Model);

    //Entity player = {};
    //player.SetModel(model);
}

bool Game::RunFrame()
{
    m_Renderer->RenderBegin();

    ImGui::ShowDemoWindow();

    ImGui::Begin("Textures");
    ImGui::Text("We try to do something cool here");
    std::vector<ITexture*> gpuTexHandles = m_Renderer->GetTextureHandles(m_Model.gpuModelHandle);
    for (auto& handle : gpuTexHandles) {
        ImGui::Text("%s", handle->m_Filename.c_str());
        ImGui::Image(
            (void*)(handle->m_hGPU),
            ImVec2(400, 400)
        );
    }
    ImGui::End();

    m_Renderer->Render();

    m_Renderer->RenderEnd();

    return true;
}

void Game::Shutdown()
{
    m_Renderer->Shutdown();
    delete m_Renderer;
}
