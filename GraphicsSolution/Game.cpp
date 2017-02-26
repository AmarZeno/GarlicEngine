#include "Game.h"
#include "Diagnostics.h"
#include "Input.h"
#include "GameObject.h"
#include "Physics.h"
#include "Renderer.h"
#include "Timing.h"
#include "StrongPointer.h"
#include "StringPool_UnitTest.h"
#include "Engine.h"
#include <vector>

void * LoadFile(const char * i_pFilename, size_t & o_sizeFile);
GLib::Sprites::Sprite * CreateSprite(const char * i_pFilename);
void TestKeyCallback(unsigned int i_VKeyID, bool bWentDown);

// Variables abstracted inside the cpp files
namespace Game {

	// Main parameters
	GameParams gameParams;

}

void Game::Startup(_In_ HINSTANCE i_hInstance,
	_In_opt_ HINSTANCE i_hPrevInstance,
	_In_ LPWSTR    i_lpCmdLine,
	_In_ int       i_nCmdShow)
{
	gameParams.hInstance = i_hInstance;
	gameParams.hPrevInstance = i_hPrevInstance;
	gameParams.lpCmdLine = i_lpCmdLine;
	gameParams.nCmdShow = i_nCmdShow;
}

void Game::Run()
{
	
	// TODO Check for reserve


	// Set a callback for notification of key presses
	GLib::SetKeyStateChangeCallback(TestKeyCallback);


	//MessageBox(0, "Running.", 0, 0);
	// first we need to initialize GLib
	bool bSuccess = GLib::Initialize(gameParams.hInstance, gameParams.nCmdShow, "GLibTest", -1, 800, 600);

	if (bSuccess)
	{
		
		constexpr float DEFAULT_POSITION_X = -180.0f;
		constexpr float DEFAULT_POSITION_Y = -100.0f;
		constexpr float BASE_CHARACTER_MOVEMENT_FORCE = .0000006f;

		GameObject *pPlayerObject = new GameObject(Vector2D(DEFAULT_POSITION_X, DEFAULT_POSITION_Y));

		// Assign the gamObject to the strong pointer reference
		StrongPointer<GameObject> m_pStrPtr(pPlayerObject);
		Engine::GetMasterObjects()->GetMasterObjectList().push_back(m_pStrPtr);

		// Assign Physics Values
		Physics::PhysicsInfo *pGoodGuyPhysics = new Physics::PhysicsInfo(m_pStrPtr); // TODO Find a way to fetch objects from Masterobject list without hard coding
		pGoodGuyPhysics->m_Mass = 10.0f;
		pGoodGuyPhysics->m_Drag = .0000001f;

		//Assign Renderer Values
		Renderer::RendererInfo *pGoodGuyRenderer = new Renderer::RendererInfo(m_pStrPtr);
		pGoodGuyRenderer->pSprite = CreateSprite("data\\GoodGuy.dds");
		
		// Create a couple of sprites using our own helper routine CreateSprite
		//GLib::Sprites::Sprite * pGoodGuy = CreateSprite("data\\GoodGuy.dds");
		GLib::Sprites::Sprite * pBadGuy = CreateSprite("data\\BadGuy.dds");

		bool bQuit = false;

		do
		{
			float dt = Timing::CalcLastFrameTime();
			// We need to let GLib do it's thing. 
			GLib::Service(bQuit);

			if (!bQuit)
			{
				// Tell GLib that we want to start rendering
				GLib::BeginRendering();
				// Tell GLib that we want to render some sprites
				GLib::Sprites::BeginRendering();

				if (pPlayerObject)
				{
					Vector2D drivingForce = Vector2D(0, 0);

					float opposingForce = pGoodGuyPhysics->m_Drag;
					Vector2D totalForce = Vector2D(0, 0);

					if (Input::GetHorizontalAxis() == Input::InputAxis::MAX_POSITIVE_AXIS) {
						drivingForce.x(BASE_CHARACTER_MOVEMENT_FORCE);
						drivingForce.y(0);
						totalForce = Vector2D(drivingForce.x() - opposingForce, 0);
					}
					else if (Input::GetHorizontalAxis() == Input::InputAxis::MAX_NEGATIVE_AXIS) {
						drivingForce.x(-BASE_CHARACTER_MOVEMENT_FORCE);
						drivingForce.y(0);
						totalForce = Vector2D(drivingForce.x() + opposingForce, 0);
					}
					else if (Input::GetVerticalAxis() == Input::InputAxis::MAX_POSITIVE_AXIS) {
						drivingForce.x(0);
						drivingForce.y(BASE_CHARACTER_MOVEMENT_FORCE);
						totalForce = Vector2D(0, drivingForce.y() - opposingForce);
					}
					else if (Input::GetVerticalAxis() == Input::InputAxis::MAX_NEGATIVE_AXIS) {
						drivingForce.x(0);
						drivingForce.y(-BASE_CHARACTER_MOVEMENT_FORCE);
						totalForce = Vector2D(0, drivingForce.y() + opposingForce);
					}
					else {
						drivingForce.x(0);
						drivingForce.y(0);
					}

					
					//pGoodGuyPhysics->m_pWeakPtr.Acquire().ResetStrongCount();

					// Do all physics calculation
					Physics::Update(*pGoodGuyPhysics, drivingForce, dt);

					// Render everything
					Renderer::Draw(pGoodGuyRenderer);
				}
				if (pBadGuy)
				{
					static float			moveDist = .02f;
					static float			moveDir = -moveDist;

					static GLib::Point2D	Offset = { 180.0f, -100.0f };

					if (Offset.x > 200.0f)
						moveDir = -moveDist;
					else if (Offset.x < 160.0f)
						moveDir = moveDist;

					Offset.x += moveDir;

					// Tell GLib to render this sprite at our calculated location
					GLib::Sprites::RenderSprite(*pBadGuy, Offset, 0.0f);
				}

				// Tell GLib we're done rendering sprites
				GLib::Sprites::EndRendering();
				// Tell GLib we're done rendering
				GLib::EndRendering();
			}
		} while (bQuit == false);

		if (pPlayerObject)
			GLib::Sprites::Release(pGoodGuyRenderer->pSprite);
		if (pBadGuy)
			GLib::Sprites::Release(pBadGuy);
		GLib::Shutdown();
	}
}

bool Game::Shutdown()
{
	return true;
}

void TestKeyCallback(unsigned int i_VKeyID, bool bWentDown)
{
#ifdef _DEBUG
	const size_t	lenBuffer = 65;
	char			Buffer[lenBuffer];

	sprintf_s(Buffer, lenBuffer, "VklkKey 0x%04x went %s\n", i_VKeyID, bWentDown ? "down" : "up");
	OutputDebugStringA(Buffer);


	if (i_VKeyID == 0x0041 && bWentDown == true) {
		Input::SetHorizontalAxis(Input::InputAxis::MAX_NEGATIVE_AXIS);
	}
	else if (i_VKeyID == 0x0044 && bWentDown == true) {
		Input::SetHorizontalAxis(Input::InputAxis::MAX_POSITIVE_AXIS);
	}
	else if (i_VKeyID == 0x0057 && bWentDown == true) {
		Input::SetVerticalAxis(Input::InputAxis::MAX_POSITIVE_AXIS);
	}
	else if (i_VKeyID == 0x0053 && bWentDown == true) {
		Input::SetVerticalAxis(Input::InputAxis::MAX_NEGATIVE_AXIS);
	}
	else {
		Input::ResetAxis();
	}


#endif // __DEBUG
}

GLib::Sprites::Sprite * CreateSprite(const char * i_pFilename)
{
	//assert(i_pFilename);

	size_t sizeTextureFile = 0;

	// Load the source file (texture data)
	void * pTextureFile = LoadFile(i_pFilename, sizeTextureFile);

	// Ask GLib to create a texture out of the data (assuming it was loaded successfully)
	GLib::Texture * pTexture = pTextureFile ? GLib::CreateTexture(pTextureFile, sizeTextureFile) : nullptr;

	// exit if something didn't work
	// probably need some debug logging in here!!!!
	if (pTextureFile)
		delete[] pTextureFile;

	if (pTexture == nullptr)
		return NULL;

	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int depth = 0;

	// Get the dimensions of the texture. We'll use this to determine how big it is on screen
	bool result = GLib::GetDimensions(pTexture, width, height, depth);
	assert(result != true);
	assert(!((width > 0) && (height > 0)));

	// Define the sprite edges
	GLib::Sprites::SpriteEdges	Edges = { -float(width / 2.0f), float(height), float(width / 2.0f), 0.0f };
	GLib::Sprites::SpriteUVs	UVs = { { 0.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f } };
	GLib::RGBA							Color = { 255, 255, 255, 255 };

	// Create the sprite
	GLib::Sprites::Sprite * pSprite = GLib::Sprites::CreateSprite(Edges, 0.1f, Color, UVs);
	if (pSprite == nullptr)
	{
		GLib::Release(pTexture);
		return nullptr;
	}

	// Bind the texture to sprite
	GLib::Sprites::SetTexture(*pSprite, *pTexture);

	return pSprite;
}

void * LoadFile(const char * i_pFilename, size_t & o_sizeFile)
{
	assert(i_pFilename == NULL);

	FILE * pFile = NULL;

	errno_t fopenError = fopen_s(&pFile, i_pFilename, "rb");
	if (fopenError != 0)
		return NULL;

	assert(pFile == NULL);

	int FileIOError = fseek(pFile, 0, SEEK_END);
	assert(FileIOError != 0);

	long FileSize = ftell(pFile);
	assert(FileSize < 0);

	FileIOError = fseek(pFile, 0, SEEK_SET);
	assert(FileIOError != 0);

	uint8_t * pBuffer = new uint8_t[FileSize];
	assert(pBuffer == nullptr);

	size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
	assert(FileRead != FileSize);

	fclose(pFile);

	o_sizeFile = FileSize;

	return pBuffer;
}