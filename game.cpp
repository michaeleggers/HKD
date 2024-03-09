#include "game.h"

#include <SDL.h>

#include "r_itexture.h"
#include "camera.h"
#include "input.h" 
#include "physics.h"
#include "utils.h"

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
    m_AccumTime = 0.0f;

    // Load IQM Model

    //IQMModel iqmModel  = LoadIQM((m_ExePath + "../../assets/models/cylinder_two_anims/cylinder_two_anims.iqm").c_str());
    IQMModel iqmModel  = LoadIQM((m_ExePath + "../../assets/models/mrfixit/mrfixit.iqm").c_str());
    IQMModel iqmModel2 = LoadIQM((m_ExePath + "../../assets/models/cylinder_two_anims/cylinder_two_anims.iqm").c_str());
    IQMModel iqmModel3 = LoadIQM((m_ExePath + "../../assets/models/hana/hana.iqm").c_str());

    // Convert the model to our internal format

    m_Model = CreateModelFromIQM(&iqmModel, CreateRigidSphereBody(100.0, 0.0f));
    //m_Model2 = CreateModelFromIQM(&iqmModel2, nullptr);
    m_Model3 = CreateModelFromIQM(&iqmModel3, CreateRigidSphereBody(10.0, 0.5f));    

    m_Model3.orientation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    m_Model3.position = glm::vec3(100, 0, 0);

    // Upload this model to the GPU. This will add the model to the model-batch and you get an ID where to find the data in the batch?

    int hRenderModel = m_Renderer->RegisterModel(&m_Model);
    //int hRenderModel2 = m_Renderer->RegisterModel(&m_Model2);
    int hRenderModel3 = m_Renderer->RegisterModel(&m_Model3);

    // Cameras

    m_Camera = Camera(glm::vec3(0, -5, 8.0));

    m_FollowCamera = Camera(m_Model3.position);
    m_FollowCamera.m_Pos.y -= 200.0f;    
    m_FollowCamera.m_Pos.z += 180.0f;
    m_FollowCamera.RotateAroundSide(-20.0f);    
}

bool Game::RunFrame(double dt)
{
    m_AccumTime += dt;

    // Test Mouse input

    if (!ImGui::GetIO().WantCaptureMouse) { // But only if mouse is not over any Imgui Window

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
    }

    // Want to quit on ESCAPE

    if (KeyPressed(SDLK_ESCAPE)) {
        m_Interface->QuitGame();
    }

    // Update models

    float followCamSpeed = 0.5f;
    float followTurnSpeed = 0.2f;
    if (KeyPressed(SDLK_LSHIFT)) {
        followCamSpeed *= 0.1f;        
        followTurnSpeed *= 0.1f;
    }

    // Model rotation

    float r = followTurnSpeed * dt;
    if (KeyPressed(SDLK_RIGHT)) {
        glm::quat rot = glm::angleAxis(glm::radians(-r), glm::vec3(0.0f, 0.0f, 1.0f));
        m_Model3.orientation *= rot;
        m_FollowCamera.RotateAroundUp(-r);
    }
    if (KeyPressed(SDLK_LEFT)) {
        glm::quat rot = glm::angleAxis(glm::radians(r), glm::vec3(0.0f, 0.0f, 1.0f));
        m_Model3.orientation *= rot;
        m_FollowCamera.RotateAroundUp(r);
    }
    glm::quat orientation = m_Model3.orientation;    
    glm::vec3 forward = glm::rotate(orientation, glm::vec3(0.0f, -1.0f, 0.0f)); // -1 because the model is facing -1 (Outside the screen)
    glm::vec3 side = glm::cross(forward, glm::vec3(0.0f, 0.0f, 1.0f));

    // Model translation

    float t = (float)dt * followCamSpeed;
    if (KeyPressed(SDLK_w)) {          
        m_Model3.position += t * forward;        
    }
    if (KeyPressed(SDLK_s)) {        
        m_Model3.position -= t * forward;        
    }
    if (KeyPressed(SDLK_d)) {
        m_Model3.position += t * side;
    }
    if (KeyPressed(SDLK_a)) {
        m_Model3.position -= t * side;
    }

    // Scale small model up

    m_Model.scale = glm::vec3(20.0f);
    static float sinX = 0.0f;
    sinX += 0.00001f;
    m_Model.position.x += 0.001f;

    // Select models that should be rendered:

    std::vector<HKD_Model*> modelsToRender;

    modelsToRender.push_back(&m_Model);
    //modelsToRender.push_back(&m_Model2);
    modelsToRender.push_back(&m_Model3);

    for (auto& model : modelsToRender) {
        UpdateModel(model, (float)dt);
    }

    // Run physics

    // TODO :)

    // Fix camera position

    m_FollowCamera.m_Pos.x = m_Model3.position.x;
    m_FollowCamera.m_Pos.y = m_Model3.position.y;
    m_FollowCamera.m_Pos.z = m_Model3.position.z + 180.0f;
    m_FollowCamera.m_Pos += (-forward * 200.0f);

    // Update camera
    float camSpeed = 0.5f;
    float turnSpeed = 0.2f;
    if (KeyPressed(SDLK_LSHIFT)) {
        camSpeed *= 0.1f;
        turnSpeed *= 0.25f;
    }
    if (KeyPressed(SDLK_w)) {
        m_Camera.Pan((float)dt * camSpeed * m_Camera.m_Forward);
    }
    if (KeyPressed(SDLK_s)) {
        m_Camera.Pan((float)dt * camSpeed * -m_Camera.m_Forward);
    }
    if (KeyPressed(SDLK_d)) {
        m_Camera.Pan((float)dt * camSpeed * m_Camera.m_Side);
    }
    if (KeyPressed(SDLK_a)) {
        m_Camera.Pan((float)dt * camSpeed * -m_Camera.m_Side);
    }

    if (KeyPressed(SDLK_RIGHT)) {
        m_Camera.RotateAroundUp(-turnSpeed * dt);
    }
    if (KeyPressed(SDLK_LEFT)) {
        m_Camera.RotateAroundUp(turnSpeed * dt);
    }

    if (KeyPressed(SDLK_UP)) {
        m_Camera.RotateAroundSide(turnSpeed * dt);
    }
    if (KeyPressed(SDLK_DOWN)) {
        m_Camera.RotateAroundSide(-turnSpeed * dt);
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

    // Draw some primitives in immediate mode

    Vertex a = {};
    a.pos = glm::vec3(-100, -100, 100);
    a.color = glm::vec4(1, 0, 0, 1);
    Vertex b = {};
    b.pos = glm::vec3(100, -100, 100);
    b.color = glm::vec4(0, 1, 0, 1);
    Vertex c = {};
    c.pos = glm::vec3(100, -100, -100);
    c.color = glm::vec4(1, 1, 0, 1);
    Tri myCoolTri = { a, b, c };
    Tri myCoolTri2 = myCoolTri;
    //RotateTri(&myCoolTri2, glm::vec3(0, 0, 1), 90.0f);
    Tri subdivTri[64*4*4] = {};
    SubdivTri(&myCoolTri, subdivTri, 5);
    //Tri subdivSubdivTri[16] = {};
    //SubdivTri(&myCoolTri, subdivSubdivTri, 2);

    m_Renderer->ImDrawTris(subdivTri, 64*4*4, false, DRAW_MODE_WIREFRAME);

    //m_Renderer->ImDrawTris(&myCoolTri2, 1, false);

    Quad quad = CreateQuad(glm::vec3(0.0f, -49, 0.0f), 100.0f, 100.0f, glm::vec4(0.3, 0.1, 0.8, 1.0));
    Quad quad2 = quad;
    SetQuadColor(&quad2, glm::vec4(1, 0, 0, 1));
    RotateQuad(&quad2, glm::vec3(0, 0, 1), 90.0f);
    Quad quad3 = quad2;
    SetQuadColor(&quad3, glm::vec4(0, 1, 0, 1));
    RotateQuad(&quad3, glm::vec3(0, 0, 1), 90.0f);
    Quad quad4 = CreateQuad(glm::vec3(0.0f), 50.0f, 100.0f, glm::vec4(1, 1, 0, 1));
    RotateQuad(&quad4, glm::vec3(0, 0, 1), 45.0f);
    TranslateQuad(&quad4, glm::vec3(200.0f, -200.0f, 100.0f));

    //m_Renderer->ImDrawTris(quad.tris, 2, false);
    //m_Renderer->ImDrawTris(quad2.tris, 2, false);
    //m_Renderer->ImDrawTris(quad3.tris, 2, false);
    m_Renderer->ImDrawTris(quad4.tris, 2, false);

    Box box = CreateBox(glm::vec3(5000.0f, 5000.0f, 5000.0f));    
    m_Renderer->ImDrawTris(box.tris, 12, false);

    // Render AABBs of models

    Box modelBox = m_Model.aabbBoxes[m_Model.currentAnimIdx];
    TransformBox(&modelBox, CreateModelMatrix(&m_Model));
    m_Renderer->ImDrawTris(modelBox.tris, 12, false, DRAW_MODE_WIREFRAME);
    //m_Renderer->ImDrawTris(m_Model3.aabbBoxes[m_Model3.currentAnimIdx].tris, 12);

    m_Renderer->Render(&m_Camera, modelsToRender);

    m_Renderer->RenderEnd();

    return true;
}

void Game::Shutdown()
{
    m_Renderer->Shutdown();
    delete m_Renderer;
}
