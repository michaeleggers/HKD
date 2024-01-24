#include "game.h"

Game::Game(std::string exePath, IRender* renderer)
{
    m_Renderer = renderer;
    m_ExePath = exePath;
}

void Game::Init()
{
    // Load IQM Model

    IQMModel iqmModel = LoadIQM((m_ExePath + "../../assets/models/mrfixit/mrfixit.iqm").c_str());

    // Convert the model to our internal format

    HKD_Model model = CreateModelFromIQM(&iqmModel);

    // Upload this model to the GPU. This will add the model to the model-batch and you get an ID where to find the data in the batch?

    int hRenderModel = m_Renderer->RegisterModel(&model);
    // Check if texture caching works
    int hRenderModelCopy = m_Renderer->RegisterModel(&model);
}

bool Game::RunFrame()
{
    m_Renderer->RenderBegin();
    m_Renderer->Render();
    m_Renderer->RenderEnd();

    return true;
}

void Game::Shutdown()
{
    m_Renderer->Shutdown();
    delete m_Renderer;
}
