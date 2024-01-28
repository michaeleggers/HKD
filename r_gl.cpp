#include "r_gl.h"

#include <SDL.h>
#include <glad/glad.h>

#include "imgui.h"
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "r_common.h"
#include "platform.h"
#include "r_itexture.h"
#include "r_gl_batch.h"
#include "r_gl_texture.h"
#include "r_gl_texture_mgr.h"

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

bool GLRender::Init(void)
{   
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create an application window with the following settings:
    m_Window = SDL_CreateWindow(
        "HKD",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );

    m_SDL_GL_Conext = SDL_GL_CreateContext(m_Window);
    if (!m_SDL_GL_Conext) {
        SDL_Log("Unable to create GL context! SDL-Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_MakeCurrent(m_Window, m_SDL_GL_Conext);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        SDL_Log("Failed to get OpenGL function pointers via GLAD: %s\n", SDL_GetError());
        return false;
    }

    // Check that the window was successfully created

    if (m_Window == NULL) {
        // In the case that the window could not be made...
        SDL_Log("Could not create window: %s\n", SDL_GetError());
        return false;
    }

    SDL_ShowWindow(m_Window);

    // GL Vsync on
    if (SDL_GL_SetSwapInterval(1) != 0) {
        SDL_Log("Failed to enable vsync!\n");
    }
    else {
        SDL_Log("vsync enabled\n");
    }

    // Init TextureManager

    m_TextureManager = GLTextureManager::Instance();

    // Setup Imgui

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(m_Window, m_SDL_GL_Conext);
    ImGui_ImplOpenGL3_Init("#version 330");

    // ImGui Config

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Some OpenGL global settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glDepthFunc(GL_LESS);

    // Some OpenGL Info

    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    SDL_Log("%s, %s\n", vendor, renderer);

    // Create batches

    // make space for 1Mio triangles
    // TODO: What should be the upper limit?
    // With sizeof(Vertex) = 92bytes => sizeof(Tri) = 276bytes we need ~ 263MB for Models. 
    // A lot for a game in the 2000s! Our models have a tri count of maybe 3000 Tris (without weapon), which
    // is not even close to 1Mio tris.
    m_ModelBatch = new GLBatch(1000 * 1000);

    // Initialize shaders

    InitShaders();

    return true;
}

void GLRender::Shutdown(void)
{
    // Deinit ImGui

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // Close and destroy the window
    SDL_GL_DeleteContext(m_SDL_GL_Conext);
    SDL_DestroyWindow(m_Window);

    m_ModelBatch->Kill();
    delete m_ModelBatch;

    m_ModelShader->Unload();
    delete m_ModelShader;
}

int GLRender::RegisterModel(HKD_Model* model)
{ 
    GLModel gl_model = {};
    GLBatchDrawCmd drawCmd = m_ModelBatch->Add(&model->tris[0], model->tris.size());

    for (int i = 0; i < model->meshes.size(); i++) {
        HKD_Mesh* mesh = &model->meshes[i];
        GLTexture* texture = (GLTexture*)m_TextureManager->CreateTexture(mesh->textureFileName);        
        GLMesh gl_mesh = {
            .triOffset = drawCmd.offset + (int)mesh->firstTri,
            .triCount = (int)mesh->numTris,
            .texture = texture
        };
        gl_model.meshes.push_back(gl_mesh);
    }

    m_Models.push_back(gl_model);

    int gpuModelHandle = m_Models.size() - 1;
    model->gpuModelHandle = gpuModelHandle;

    return gpuModelHandle;
}

// Maybe return a void* as GPU handle, because usually APIs that use the handle of
// a specific graphics API don't expect it to be in int or whatever.
std::vector<ITexture*> GLRender::ModelTextures(int gpuModelHandle)
{
    std::vector<ITexture*> results;
    
    if (gpuModelHandle >= m_Models.size()) return results;
    else if (gpuModelHandle < 0)           return results;

    GLModel* model = &m_Models[gpuModelHandle];
    for (auto& mesh : model->meshes) {
        results.push_back( mesh.texture );
    }

    return results;
}

std::vector<ITexture*> GLRender::Textures(void)
{
    std::vector<ITexture*> result;
    for (auto& elem: m_TextureManager->m_NameToTexture) {
        result.push_back(elem.second);
    }

    return result;
}

void GLRender::RenderBegin(void)
{    
    SDL_GetWindowSize(m_Window, &m_WindowWidth, &m_WindowHeight);
    float windowAspect = (float)m_WindowWidth / (float)m_WindowHeight;
    glViewport(0, 0, m_WindowWidth, m_WindowHeight);

    glClearColor(0.2f, 0.4f, 0.7f, 1.0f); // Nice blue :)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();
}

void GLRender::Render(std::vector<HKD_Model*>& models)
{    
    // Camera and render settings

    static float x = -1.0f;
    static float y = -4.0f;
    static float z = 8.0f;
    static uint32_t drawWireframe = 0;

    ImGui::Begin("controlls");
    ImGui::Text("Cam position:");
    ImGui::SliderFloat("x", &x, -500.0f, 500.0f);
    ImGui::SliderFloat("y", &y, -500.0f, 500.0f);
    ImGui::SliderFloat("z", &z, -500.0f, 500.0f);
    ImGui::Text("Render settings:");
    ImGui::Checkbox("wireframe", (bool*)&drawWireframe);    
    ImGui::End();

    glm::mat4 view = glm::lookAt(
        glm::vec3(x, y, z),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 0, 1));
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)m_WindowWidth / (float)m_WindowHeight, 0.1f, 1000.0f);

    // Draw Models

    m_ModelBatch->Bind();
    m_ModelShader->Activate();
    m_ModelShader->DrawWireframe((uint32_t)drawWireframe);
    m_ModelShader->SetViewProjMatrices(view, proj);
    for (int i = 0; i < models.size(); i++) {
        
        GLModel model = m_Models[models[i]->gpuModelHandle];
        m_ModelShader->SetMatrixPalette(&models[i]->palette[0], models[i]->numJoints);

        for (int j = 0; j < model.meshes.size(); j++) {
            GLMesh* mesh = &model.meshes[j];            
            glBindTexture(GL_TEXTURE_2D, mesh->texture->m_gl_Handle);
            glDrawArrays(GL_TRIANGLES, 3*mesh->triOffset, 3 * mesh->triCount);
        }
    }
    //const std::vector<GLBatchDrawCmd>& modelDrawCmds = m_ModelBatch->DrawCmds();
    //for (int i = 0; i < modelDrawCmds.size(); i++) {
    //    glBindTexture(GL_TEXTURE_2D, modelDrawCmds[i].hTexture);
    //    glDrawArrays(GL_TRIANGLES, 3*modelDrawCmds[i].offset, 3 * modelDrawCmds[i].numTris);
    //}
    //glDrawArrays(GL_TRIANGLES, 0, 3*m_ModelBatch->TriCount());
}

void GLRender::RenderEnd(void)
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(m_Window);
}

void GLRender::InitShaders()
{
    std::string exePath = hkd_GetExePath();

    m_ModelShader = new Shader();
    if (!m_ModelShader->Load(
        exePath + "../../shaders/entities.vert",
        exePath + "../../shaders/entities.frag"
    )) {
        printf("Problems initializing model shaders!\n");
    }
}

