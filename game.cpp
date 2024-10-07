#include "game.h"

#include <SDL.h>

#include "r_itexture.h"
#include "camera.h"
#include "input.h" 
#include "physics.h"
#include "utils.h"
#include "Shape.h"
#include "ShapeSphere.h"
#include "CWorld.h"

#include "imgui.h"

#define NUM_BALLS 2


#define MAP_PARSER_IMPLEMENTATION
#include "map_parser.h"

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
	
	std::vector<TriPlane> worldTris{};
/*	int const numWorldTris = 1000;
	for (int i = 0; i < numWorldTris; i++) {
		TriPlane triPlane{};
		float x1 = RandBetween(-5000.0f, 500.0f);
		float x2 = RandBetween(-5000.0f, 500.0f);
		float x3 = RandBetween(-5000.0f, 500.0f);
		float y1 = RandBetween(-5000.0f, 500.0f);
		float y2 = RandBetween(-5000.0f, 500.0f);
		float y3 = RandBetween(-5000.0f, 500.0f);
		float z1 = RandBetween(-1000.0f, 5000.0f);
		float z2 = RandBetween(-1000.0f, 5000.0f);
		float z3 = RandBetween(-1000.0f, 5000.0f);
		Vertex A = {glm::vec3(x1, y1, z1)};
		Vertex B = {glm::vec3(x2, y2, z2)};
		Vertex C = {glm::vec3(x3, y3, z3)};
		glm::vec4 triPlaneColor = glm::vec4( RandBetween(0.0f, 1.0f), RandBetween(0.0f, 1.0f), RandBetween(0.0f, 1.0f), 1.0f);
		A.color = triPlaneColor;
		B.color = triPlaneColor;
		C.color = triPlaneColor;
		triPlane.tri = {A, B, C};
		triPlane.plane = CreatePlaneFromTri(triPlane.tri);
		triPlane.tri.a.normal = triPlane.plane.normal;
		triPlane.tri.b.normal = triPlane.plane.normal;
		triPlane.tri.c.normal = triPlane.plane.normal;
		worldTris.push_back( triPlane );	
	}
	*/
	TriPlane triPlane{};
	Vertex A = {glm::vec3(0.0f, 0.0f, 0.0f)};
	Vertex B = {glm::vec3(0.0f, 0.0f, 300.0f)};
	Vertex C = {glm::vec3(-3000.0f, 0.0f, 0.0f)};
	glm::vec4 triPlaneColor = glm::vec4( RandBetween(0.0f, 1.0f), RandBetween(0.0f, 1.0f), RandBetween(0.0f, 1.0f), 1.0f);
	A.color = triPlaneColor;
	B.color = triPlaneColor;
	C.color = triPlaneColor;
	Tri tri = {A, C, B};
	triPlane.tri = tri;
	triPlane.plane = CreatePlaneFromTri(triPlane.tri);
	triPlane.tri.a.normal = triPlane.plane.normal;
	triPlane.tri.b.normal = triPlane.plane.normal;
	triPlane.tri.c.normal = triPlane.plane.normal;
	worldTris.push_back( triPlane );	

	triPlaneColor = glm::vec4( RandBetween(0.0f, 1.0f), RandBetween(0.0f, 1.0f), RandBetween(0.0f, 1.0f), 1.0f);
	A.color = triPlaneColor;
	B.color = triPlaneColor;
	C.color = triPlaneColor;
	tri = { A, B, C };
	triPlane.tri = tri;
	RotateTri(&triPlane.tri, glm::vec3(0.0f, 0.0f, 1.0f), 130.0f);
	triPlane.plane = CreatePlaneFromTri(triPlane.tri);
	triPlane.tri.a.normal = triPlane.plane.normal;
	triPlane.tri.b.normal = triPlane.plane.normal;
	triPlane.tri.c.normal = triPlane.plane.normal;
	//worldTris.push_back( triPlane );	

	triPlaneColor = glm::vec4( RandBetween(0.0f, 1.0f), RandBetween(0.0f, 1.0f), RandBetween(0.0f, 1.0f), 1.0f);
	A.color = triPlaneColor;
	B.color = triPlaneColor;
	C.color = triPlaneColor;
	tri = { A, B, C };
	triPlane.tri = tri;
	RotateTri(&triPlane.tri, glm::vec3(0.0f, 0.0f, 1.0f), -110.0f);
	triPlane.plane = CreatePlaneFromTri(triPlane.tri);
	triPlane.tri.a.normal = triPlane.plane.normal;
	triPlane.tri.b.normal = triPlane.plane.normal;
	triPlane.tri.c.normal = triPlane.plane.normal;
	worldTris.push_back( triPlane );	
   
	// Floor with wall
	float stepHeight = 10.0f;
	glm::vec4 colorFloor = glm::vec4(1.0f, 0.2f, 0.2f, 1.0f);
	triPlaneColor = glm::vec4( RandBetween(0.0f, 1.0f), RandBetween(0.0f, 1.0f), RandBetween(0.0f, 1.0f), 1.0f);
	Tri floorTri;
	A = {glm::vec3(0.0f, 0.0f, stepHeight)};
	B = {glm::vec3(0.0f, 300.0f, stepHeight)};
	C = {glm::vec3(-300.0f, 300.0f, stepHeight)};
	A.color = colorFloor;
	B.color = colorFloor;
	C.color = colorFloor;
	floorTri = { A, B, C };
	triPlane.tri = floorTri;
	triPlane.plane = CreatePlaneFromTri(triPlane.tri);
	triPlane.tri.a.normal = triPlane.plane.normal;
	triPlane.tri.b.normal = triPlane.plane.normal;
	triPlane.tri.c.normal = triPlane.plane.normal;
	worldTris.push_back( triPlane );	

	Tri wallTri;
	A = {glm::vec3(0.0f, 300.0f, stepHeight)};
	B = {glm::vec3(-300.0f, 300.0f, stepHeight)};
	C = {glm::vec3(-300.0f, 300.0f, -300.0f)};
	A.color = triPlaneColor;
	B.color = triPlaneColor;
	C.color = triPlaneColor;
	wallTri = { A, C, B };
	triPlane.tri = wallTri;
	triPlane.plane = CreatePlaneFromTri(triPlane.tri);
	triPlane.tri.a.normal = triPlane.plane.normal;
	triPlane.tri.b.normal = triPlane.plane.normal;
	triPlane.tri.c.normal = triPlane.plane.normal;
	worldTris.push_back( triPlane );	

	// Second step
	TranslateTri( &floorTri, glm::vec3( 0.0f, -100.0f, stepHeight ) );
	triPlane.tri = floorTri;
	triPlane.plane = CreatePlaneFromTri(triPlane.tri);
	triPlane.tri.a.normal = triPlane.plane.normal;
	triPlane.tri.b.normal = triPlane.plane.normal;
	triPlane.tri.c.normal = triPlane.plane.normal;
	worldTris.push_back( triPlane );	

	TranslateTri( &wallTri, glm::vec3( 0.0f, -100.0f, stepHeight ) );
	triPlane.tri = wallTri;
	triPlane.plane = CreatePlaneFromTri(triPlane.tri);
	triPlane.tri.a.normal = triPlane.plane.normal;
	triPlane.tri.b.normal = triPlane.plane.normal;
	triPlane.tri.c.normal = triPlane.plane.normal;
	worldTris.push_back( triPlane );	

	Tri groundTri;
	A = {glm::vec3(2000.0f, -100000.0f, -20.0f)};
	B = {glm::vec3(2000.0f, 1000.0f, -20.0f)};
	C = {glm::vec3(-1000.0f, 1000.0f, -20.0f)};
	A.color = triPlaneColor;
	B.color = triPlaneColor;
	C.color = triPlaneColor;
	groundTri = { A, B, C };
	triPlane.tri = groundTri;
	triPlane.plane = CreatePlaneFromTri(triPlane.tri);
	triPlane.tri.a.normal = triPlane.plane.normal;
	triPlane.tri.b.normal = triPlane.plane.normal;
	triPlane.tri.c.normal = triPlane.plane.normal;
	worldTris.push_back( triPlane );	

	m_World.InitWorld(worldTris.data(), worldTris.size());

    // Load IQM Model

    //IQMModel iqmModel  = LoadIQM((m_ExePath + "../../assets/models/cylinder_two_anims/cylinder_two_anims.iqm").c_str());
    IQMModel iqmModel  = LoadIQM("models/multiple_anims/multiple_anims.iqm");
    IQMModel iqmModelMrFixit = LoadIQM("models/mrfixit/mrfixit.iqm");
    IQMModel iqmModel2 = LoadIQM("models/cylinder_two_anims/cylinder_two_anims.iqm");
    IQMModel iqmModel3 = LoadIQM("models/hana/hana.iqm");

    IQMModel iqmIcosphere = LoadIQM("models/icosphere/icosphere.iqm");
    IQMModel iqmIcosphereHighRes = LoadIQM("models/icosphere/icosphere_low_res.iqm");

    // Convert the model to our internal format

    m_Model = CreateModelFromIQM(&iqmModelMrFixit);
    //m_Model2 = CreateModelFromIQM(&iqmModel2, nullptr);
    m_Model3 = CreateModelFromIQM(&iqmModel3);    
    m_Player = CreateModelFromIQM(&iqmModel);
    m_Player.isRigidBody = false;

    for (int i = 0; i < NUM_BALLS; i++) {
        HKD_Model icosphereModel = CreateModelFromIQM(&iqmIcosphere);
        float icosphereRadius = 100.0f;
        icosphereModel.scale = glm::vec3(icosphereRadius);
        float posX = RandBetween(-200.0f, 200.0f);
        float posY = RandBetween(-200.0f, 200.0f);
        float posZ = RandBetween(600.0f, 3000.0f);
        icosphereModel.position = glm::vec3(posX, posY, posZ);
        Body icosphereBody;
        icosphereBody.m_Position = icosphereModel.position;
        icosphereBody.m_Orientation = glm::angleAxis(
            glm::radians(0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f));
        icosphereBody.m_LinearVelocity = glm::vec3(0.0f);
        icosphereBody.m_Shape = new ShapeSphere(icosphereRadius);
        icosphereBody.m_InvMass = 1.0f / 10.0f;
        icosphereBody.m_Elasticity = .5f;
        icosphereModel.body = icosphereBody;
        icosphereModel.isRigidBody = true;
        m_IcosphereModels.push_back(icosphereModel);
    }
    for (int i = 0; i < NUM_BALLS; i++) {
        phys_AddBody(&m_IcosphereModels[i].body);
    }

    m_IcosphereGroundModel = CreateModelFromIQM(&iqmIcosphereHighRes);
    float groundRadius = 1000.0f;
    m_IcosphereGroundModel.scale = glm::vec3(groundRadius);
    m_IcosphereGroundModel.position = glm::vec3(0.0f, 0.0f, -groundRadius);
    Body groundBody;
    groundBody.m_Position = m_IcosphereGroundModel.position;
    groundBody.m_LinearVelocity = glm::vec3(0.0f);
    groundBody.m_Shape = new ShapeSphere(groundRadius);
    groundBody.m_InvMass = 0.0f;
    groundBody.m_Elasticity = 1.0f;
    m_IcosphereGroundModel.body = groundBody;
    m_IcosphereGroundModel.isRigidBody = true;
    phys_AddBody(&m_IcosphereGroundModel.body);

    m_Model.position = glm::vec3(0.0f, 0.0f, 100.0f);
    m_Model3.orientation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    m_Model3.position = glm::vec3(100, 0, 0);

    m_Player.position = glm::vec3(100, 300.0f, 0);
    m_Player.scale = glm::vec3(50.0f);
    for (int i = 0; i < m_Player.animations.size(); i++) {
        EllipsoidCollider* ec = &m_Player.ellipsoidColliders[i];
        ec->radiusA *= m_Player.scale.x;
        ec->radiusB *= m_Player.scale.z;
        ec->center = m_Player.position + glm::vec3(
            0.0f,
            0.0f,
            ec->radiusB);
        glm::vec3 scale = glm::vec3(1.0f / ec->radiusA, 1.0f / ec->radiusA, 1.0f / ec->radiusB);
        ec->toESpace = glm::scale(glm::mat4(1.0f), scale);
    }

    SetAnimState(&m_Player, ANIM_STATE_WALK);

    // Upload this model to the GPU. This will add the model to the model-batch and you get an ID where to find the data in the batch?

    int hRenderModel = m_Renderer->RegisterModel(&m_Model);
    //int hRenderModel2 = m_Renderer->RegisterModel(&m_Model2);
    int hRenderModel3 = m_Renderer->RegisterModel(&m_Model3);
    int hPlayerModel = m_Renderer->RegisterModel(&m_Player);
    for (int i = 0; i < NUM_BALLS; i++) {
        int hRenderIcosphere = m_Renderer->RegisterModel(&m_IcosphereModels[i]);
    }
    int hRenderIcosphereGround = m_Renderer->RegisterModel(&m_IcosphereGroundModel);

    for (int i = 0; i < 10; i++) {
        HKD_Model model = m_Model;
        model.position = glm::vec3(RandBetween(500.0f, 3000.0f), RandBetween(500.0f, 3000.0f), 0.0f);
        model.scale = glm::vec3(RandBetween(20.0f, 100.0f));
        model.orientation = glm::angleAxis(glm::radians(RandBetween(0.0f, 360.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
        model.currentFrame = (int)RandBetween(0.0f, (float)(model.numFrames - 1));
        m_FixitModels.push_back(model);
    }

    // Primitives

    m_Box = CreateBox(glm::vec3(100.0f, 100.0f, 100.0f), glm::vec4(1.0f, 0.9f, 0.0f, 1.0));
    TranslateBox(&m_Box, glm::vec3(100.0f, 100.0f, 100.0f));
    m_SkyBox = CreateBox(glm::vec3(7000.0f, 7000.0f, 7000.0f), glm::vec4(0.4f, 0.1f, 1.0f, 1.0));

    // Cameras

    m_Camera = Camera(glm::vec3(0, -1000.0f, 600.0));
    m_Camera.RotateAroundSide(-15.0f);

    m_FollowCamera = Camera(m_Player.position);
    m_FollowCamera.m_Pos.y -= 200.0f;
    m_FollowCamera.m_Pos.z += 100.0f;
    m_FollowCamera.RotateAroundSide(-20.0f);    
    m_FollowCamera.RotateAroundUp(180.0f);
}

static void DrawCoordinateSystem(IRender * renderer) {
    Vertex origin = {glm::vec3(0.0f)};
    origin.color = glm::vec4(1.0f);
    Vertex X = {glm::vec3(100.0f, 0.0f, 0.0f)};
    X.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    Vertex Y = {glm::vec3(0.0f, 100.0f, 0.0f)};
    Y.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    Vertex Z = {glm::vec3(0.0f, 0.0f, 100.0f)};
    Z.color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    Vertex OX[] = {origin, X};
    Vertex OY[] = {origin, Y};
    Vertex OZ[] = {origin, Z};
    renderer->ImDrawLines(OX, 2);
    renderer->ImDrawLines(OY, 2);
    renderer->ImDrawLines(OZ, 2);
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
        followTurnSpeed *= 0.3f;
    }

    // Model rotation

    float r = followTurnSpeed * dt;
    if (KeyPressed(SDLK_RIGHT)) {
        glm::quat rot = glm::angleAxis(glm::radians(-r), glm::vec3(0.0f, 0.0f, 1.0f));
        m_Player.orientation *= rot;
        m_FollowCamera.RotateAroundUp(-r);
    }
    if (KeyPressed(SDLK_LEFT)) {
        glm::quat rot = glm::angleAxis(glm::radians(r), glm::vec3(0.0f, 0.0f, 1.0f));
        m_Player.orientation *= rot;
        m_FollowCamera.RotateAroundUp(r);
    }
    glm::quat orientation = m_Player.orientation;
    glm::vec3 forward = glm::rotate(orientation, glm::vec3(0.0f, -1.0f, 0.0f)); // -1 because the model is facing -1 (Outside the screen)
    glm::vec3 side = glm::cross(forward, glm::vec3(0.0f, 0.0f, 1.0f));

    // Model translation

    m_Player.velocity = glm::vec3(0.0f);
    float t = (float)dt * followCamSpeed;
    AnimState playerAnimState = ANIM_STATE_IDLE;
    if (KeyPressed(SDLK_w)) {          
        m_Player.velocity += t * forward;
        playerAnimState = ANIM_STATE_RUN;
    }
    if (KeyPressed(SDLK_s)) {        
        m_Player.velocity -= t * forward;
        playerAnimState = ANIM_STATE_RUN;
    }
    if (KeyPressed(SDLK_d)) {
        m_Player.velocity += t * side;
        playerAnimState = ANIM_STATE_RUN;        
    }
    if (KeyPressed(SDLK_a)) {
        m_Player.velocity -= t * side;
        playerAnimState = ANIM_STATE_RUN;
    }

    if (playerAnimState == ANIM_STATE_RUN) {
        if (KeyPressed(SDLK_LSHIFT)) {
            playerAnimState = ANIM_STATE_WALK;
        }
    }
	//m_Player.velocity = t * glm::normalize(m_Player.velocity);
	//printf("player vel: %f, %f, %f\n", m_Player.velocity.x, m_Player.velocity.y, m_Player.velocity.z);

    SetAnimState(&m_Player, playerAnimState);

    /*
    for (auto& model : modelsToRender) {
        UpdateModel(model, (float)dt);
    }
    for (auto& model : m_FixitModels) {
        UpdateModel(&model, (float)dt);
    }
    */

    // Run physics

    phys_Update((float)dt / 100.0f);

    // Test collision between player and world geometry
    EllipsoidCollider ec = m_Player.ellipsoidColliders[m_Player.currentAnimIdx];
    CollisionInfo collisionInfo = CollideEllipsoidWithTriPlane(
			ec, m_Player.velocity, m_World.m_TriPlanes.data(), m_World.m_TriPlanes.size());
    TriPlane tp = m_World.m_TriPlanes[0];
    Plane p = CreatePlaneFromTri(tp.tri);
    // if (IsPointInTriangle(ec.center, tp.tri, p.normal)) {
    //     printf("Point in Triangle\n");
    // }
    // else {
    //     printf("POINT NOT IN TRIANGLE.\n");
    // }

    // if (collisionInfo.didCollide) {
    //     if (collisionInfo.t <= 1.0f) {
    //         m_Player.velocity = collisionInfo.t * collisionInfo.normal;
    //     } else {
    //         m_Player.velocity = collisionInfo.t * m_Player.velocity;
    //     }
    // }

    // And apply the velocity
    //m_Player.position += m_Player.velocity;
	for (int i = 0; i < m_Player.animations.size(); i++) {
		m_Player.ellipsoidColliders[ i ].center = collisionInfo.basePos;
	}
    m_Player.position.x = collisionInfo.basePos.x;
	m_Player.position.y = collisionInfo.basePos.y;
	m_Player.position.z = collisionInfo.basePos.z - ec.radiusB;

    UpdateModel(&m_Player, (float)dt);
    for (int i = 0; i < NUM_BALLS; i++) {
        UpdateModel(&m_IcosphereModels[i], (float)dt);
    }
    UpdateModel(&m_IcosphereGroundModel, (float)dt);
    //UpdateModel(&m_IcosphereModel, (float)dt);

    // Fix camera position

    m_FollowCamera.m_Pos.x = m_Player.position.x;
    m_FollowCamera.m_Pos.y = m_Player.position.y;
    m_FollowCamera.m_Pos.z = m_Player.position.z + 160.0f;
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

    // Display textures
#if 0
    static std::vector<bool> showModelInspector;
    std::vector<ITexture*> textures = m_Renderer->Textures();
    showModelInspector.resize(textures.size());

    int textureID = 0;

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
#endif
    
#if 0
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
#endif

    #define NUM_POINTS 32

#if 1
    // Draw some primitives in immediate mode

    // Draw Debug Line for player veloctiy vector
    Line velocityDebugLine = {
        Vertex(ec.center), Vertex(ec.center + 200.0f * m_Player.velocity)
    };
    velocityDebugLine.a.color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    velocityDebugLine.b.color = velocityDebugLine.a.color;
    m_Renderer->ImDrawLines(velocityDebugLine.vertices, 2, false);


    // Render World geometry
    m_Renderer->ImDrawTriPlanes(m_World.m_TriPlanes.data(), m_World.m_TriPlanes.size(),
        true, DRAW_MODE_SOLID);

    Vertex a = {};
    a.pos = glm::vec3(100, -300, -100);
    a.color = glm::vec4(1, 0, 0, 1);
    a.normal = glm::vec3(0.0f, -1.0f, 0.0f);
    Vertex b = {};
    b.pos = glm::vec3(100, -300, 100);
    b.color = glm::vec4(0, 1, 0, 1);
    b.normal = glm::vec3(0.0f, -1.0f, 0.0f);
    Vertex c = {};
    c.pos = glm::vec3(-100, -300, 100);
    c.color = glm::vec4(1, 1, 0, 1);
    c.normal = glm::vec3(0.0f, -1.0f, 0.0f);
    Tri myCoolTri = { a, b, c };
    Tri myCoolTri2 = myCoolTri;
    //RotateTri(&myCoolTri2, glm::vec3(0, 0, 1), 90.0f);
    Tri subdivTri[4] = {};
    SubdivTri(&myCoolTri, subdivTri, 1);
    //Tri subdivSubdivTri[16] = {};
    //SubdivTri(&myCoolTri, subdivSubdivTri, 2);

    m_Renderer->ImDrawTris(subdivTri, 4, true, DRAW_MODE_WIREFRAME);

    //m_Renderer->ImDrawTris(&myCoolTri2, 1, false);

    Quad quadXZ = CreateQuad(glm::vec3(0, 0.0f, 0.0f), 100.0f * GOLDEN_RATIO, 100.0f);
    Quad quadYZ = quadXZ;
    RotateQuad(&quadYZ, glm::vec3(0.0f, 0.0f, 1.0f), 90.0f);
    RotateQuad(&quadYZ, glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
    SetQuadColor(&quadYZ, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    Quad quadXY = quadXZ;
    RotateQuad(&quadXY, glm::vec3(1.0f, 0.0f, 0.0f), -90.0f);
    RotateQuad(&quadXY, glm::vec3(0.0f, 0.0f, 1.0f), 90.0f);
    SetQuadColor(&quadXY, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    FaceQuad fqXZ = QuadToFace(&quadXZ);
    FaceQuad fqYZ = QuadToFace(&quadYZ);
    FaceQuad fqXY = QuadToFace(&quadXY);

    // Use all the facequad points to create an icosahedron 

    m_Renderer->ImDrawTris(quadXZ.tris, 2, true);
    m_Renderer->ImDrawTris(quadYZ.tris, 2, true);
    m_Renderer->ImDrawTris(quadXY.tris, 2, true);

    m_Renderer->ImDrawLines(fqXZ.vertices, 4, true);
    m_Renderer->ImDrawLines(fqYZ.vertices, 4, true);
    m_Renderer->ImDrawLines(fqXY.vertices, 4, true);

    //m_Renderer->ImDrawTris(m_Box.tris, 12, true);
    m_Renderer->ImDrawTris(m_SkyBox.tris, 12, false);

    NBox nbox = CreateNBox(glm::vec3(500.0f, 200.0f, 700.0f), 1);
    m_Renderer->ImDrawTris(nbox.tris.data(), nbox.tris.size(), false, DRAW_MODE_WIREFRAME);

    // A circle
    float redIncrement = 1.0f / (float)NUM_POINTS;
    float sliceAngle = 2 * HKD_PI / (float)NUM_POINTS;
    float radius = 77.0f;
    Vertex circleVertices[NUM_POINTS];
    for (int i = 0; i < NUM_POINTS; i++) {
        Vertex v = {};
        v.pos.y = 0.0f;
        v.pos.x = 200.0f + 100.0f * cosf(i * sliceAngle);
        v.pos.z = 200.0f + 200.0f * sinf(i * sliceAngle);
        v.color = glm::vec4(redIncrement*i, 0.4f, 0.2f, 1.0f);
        circleVertices[i] = v;
    }
                            // vert-data    // vert-count  // connect start and end point?
    m_Renderer->ImDrawLines(circleVertices, NUM_POINTS,    true);

    /*
    // Draw indexed geometry

    Vertex indexedQuadVerts[4];
    indexedQuadVerts[0].pos = glm::vec3(0.0f, 200.0f, 0.0f);
    indexedQuadVerts[0].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    indexedQuadVerts[1].pos = glm::vec3(400.0f, 200.0f, 700.0f);
    indexedQuadVerts[1].color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    indexedQuadVerts[2].pos = glm::vec3(900.0f, 200.0f, 0.0f);
    indexedQuadVerts[2].color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    indexedQuadVerts[3].pos = glm::vec3(0.0f, 200.0f, -700.0f);
    indexedQuadVerts[3].color = glm::vec4(1.0f, 0.5f, 1.0f, 1.0f);

    uint16_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    m_Renderer->ImDrawIndexed(indexedQuadVerts, 4, indices, 6, true, DRAW_MODE_SOLID);

    indexedQuadVerts[0].pos = glm::vec3(0.0f, 600.0f, 0.0f);
    indexedQuadVerts[0].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    indexedQuadVerts[1].pos = glm::vec3(400.0f, 600.0f, 700.0f);
    indexedQuadVerts[1].color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    indexedQuadVerts[2].pos = glm::vec3(900.0f, 600.0f, 0.0f);
    indexedQuadVerts[2].color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    indexedQuadVerts[3].pos = glm::vec3(0.0f, 600.0f, -700.0f);
    indexedQuadVerts[3].color = glm::vec4(1.0f, 0.5f, 1.0f, 1.0f);

    m_Renderer->ImDrawIndexed(indexedQuadVerts, 4, indices, 6, false, DRAW_MODE_SOLID);

    // Indexed Triangle subidvided

    Vertex triangleVerts[3];
    triangleVerts[0].pos = glm::vec3(100.0f, 100.0f, 100.0f);
    triangleVerts[1].pos = glm::vec3(500.0f, 100.0f, 600.0f);
    triangleVerts[2].pos = glm::vec3(900.0f, 100.0f, 100.0f);

    uint16_t triangleIndices[3] = { 0, 1, 2 };

    Vertex subdivIndexedVerts[6];
    uint16_t subdivIndexedIndices[12];
    SubdivIndexedTri(triangleVerts, 3, triangleIndices, 3, subdivIndexedVerts, subdivIndexedIndices);

    m_Renderer->ImDrawIndexed(subdivIndexedVerts, 6, subdivIndexedIndices, 12, false, DRAW_MODE_WIREFRAME);
*/
#endif

#if 1
    // Render AABBs of models

    Box modelBox = m_Model.aabbBoxes[m_Model.currentAnimIdx];
    TransformBox(&modelBox, CreateModelMatrix(&m_Model));
    m_Renderer->ImDrawTris(modelBox.tris, 12, false, DRAW_MODE_WIREFRAME);

    for (int i = 0; i < m_FixitModels.size(); i++) {
        HKD_Model* m = &m_FixitModels[i];
        Box b = m->aabbBoxes[m->currentAnimIdx];
        TransformBox(&b, CreateModelMatrix(m));
        m_Renderer->ImDrawTris(b.tris, 12, false, DRAW_MODE_WIREFRAME);
    }

#endif

    DrawCoordinateSystem(m_Renderer);

    HKD_Model* renderModels[NUM_BALLS + 2];
    for (int i = 0; i < NUM_BALLS; i++) {
        renderModels[i] = &m_IcosphereModels[i];
    }
    renderModels[NUM_BALLS] = &m_Player;
    renderModels[NUM_BALLS + 1] = &m_IcosphereGroundModel;

    m_Renderer->Render(
        &m_FollowCamera,
        renderModels, NUM_BALLS + 2);

    if (collisionInfo.didCollide) {
        m_Player.debugColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // red
        m_Renderer->SetActiveCamera(&m_FollowCamera);
        m_Renderer->ImDrawSphere(collisionInfo.hitPoint, 5.0f);
		//printf("hitpoint: %f, %f, %f\n", collisionInfo.hitPoint.x, collisionInfo.hitPoint.y, collisionInfo.hitPoint.z);
    } else {
        m_Player.debugColor = glm::vec4(1.0f); // white
    }
    m_Renderer->SetActiveCamera(&m_FollowCamera);
    //m_Renderer->ImDrawSphere(ec.center, 5.0f);
    HKD_Model* playerColliderModel[] = {&m_Player};
    m_Renderer->RenderColliders(&m_FollowCamera, playerColliderModel, 1);

    m_Renderer->RenderEnd();

    return true;
}

void Game::Shutdown()
{
    m_Renderer->Shutdown();
    delete m_Renderer;
}
