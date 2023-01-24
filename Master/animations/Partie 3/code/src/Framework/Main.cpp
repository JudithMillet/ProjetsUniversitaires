
#include <glew/glew.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "DebugDraw.h"

#include "Application.h"

#include "glfw/glfw3.h"
#include <stdio.h>

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#ifdef _WIN32
#ifdef _MSC_VER
#include <crtdbg.h>
#else
#define _ASSERT(expr) ((void)0)
#define _ASSERTE(expr) ((void)0)
#endif
#endif

struct UIState {bool showMenu;};

namespace {
	GLFWwindow* mainWindow = NULL;
	UIState ui;
	Application* app;
	Settings settings;
	bool rightMouseDown;
	b2Vec2 lastp;
}

static void sCreateUI(GLFWwindow* window) {
	ui.showMenu = true;
	// Init UI
	const char* fontPath = "src/Data/DroidSans.ttf";
	ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath, 20.f);
	if (ImGui_ImplGlfwGL3_Init(window, false) == false)	{fprintf(stderr, "Could not init GUI renderer.\n");assert(false);return;}

	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = style.GrabRounding = style.ScrollbarRounding = 2.0f;
	style.FramePadding = ImVec2(4, 2);
	style.DisplayWindowPadding = ImVec2(0, 0);
	style.DisplaySafeAreaPadding = ImVec2(0, 0);
}

static void sResizeWindow(GLFWwindow*, int width, int height) {
	g_camera.m_width = width;
	g_camera.m_height = height;
}

static void sKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);
	bool keys_for_ui = ImGui::GetIO().WantCaptureKeyboard;
	if (keys_for_ui) return;

	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE: // Quit
			glfwSetWindowShouldClose(mainWindow, GL_TRUE);
			break;

		case GLFW_KEY_LEFT: // Pan left
			if (mods == GLFW_MOD_CONTROL) {
				b2Vec2 newOrigin(2.0f, 0.0f);
				app->ShiftOrigin(newOrigin);
			}
			else {g_camera.m_center.x -= 0.5f;}
			break;

		case GLFW_KEY_RIGHT: // Pan right
			if (mods == GLFW_MOD_CONTROL) {
				b2Vec2 newOrigin(-2.0f, 0.0f);
				app->ShiftOrigin(newOrigin);
			}
			else {g_camera.m_center.x += 0.5f;}
			break;

		case GLFW_KEY_DOWN: // Pan down
			if (mods == GLFW_MOD_CONTROL) {
				b2Vec2 newOrigin(0.0f, 2.0f);
				app->ShiftOrigin(newOrigin);
			}
			else {g_camera.m_center.y -= 0.5f;}
			break;

		case GLFW_KEY_UP: // Pan up
			if (mods == GLFW_MOD_CONTROL) {
				b2Vec2 newOrigin(0.0f, -2.0f);
				app->ShiftOrigin(newOrigin);
			}
			else {g_camera.m_center.y += 0.5f;}
			break;

		case GLFW_KEY_HOME: // Reset view
			g_camera.m_zoom = 1.0f;
			g_camera.m_center.Set(0.0f, 20.0f);
			break;

		case GLFW_KEY_PAGE_DOWN: // Zoom out
			g_camera.m_zoom = b2Min(1.1f * g_camera.m_zoom, 20.0f);
			break;

		case GLFW_KEY_PAGE_UP: // Zoom in
			g_camera.m_zoom = b2Max(0.9f * g_camera.m_zoom, 0.02f);
			break;

		case GLFW_KEY_R: // Reset app
			delete app;
			app = new Application;
			break;

		case GLFW_KEY_SPACE: // Launch a ball
			if (app) {app->LaunchBall();}
			break;

		/*case GLFW_KEY_P: // Pause
			settings.pause = !settings.pause;
			break;*/

		case GLFW_KEY_TAB: // Show menu
			ui.showMenu = !ui.showMenu;

		default:
			if (app) {app->Keyboard(key);}
		}
	}
	else if (action == GLFW_RELEASE) {app->KeyboardUp(key);}
}

static void sCharCallback(GLFWwindow* window, unsigned int c) {
	ImGui_ImplGlfwGL3_CharCallback(window, c);
}

static void sMouseButton(GLFWwindow* window, int32 button, int32 action, int32 mods) {
	ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);

	double xd, yd;
	glfwGetCursorPos(mainWindow, &xd, &yd);
	b2Vec2 ps((float32)xd, (float32)yd);

	// Use the mouse to move things around.
	if (button == GLFW_MOUSE_BUTTON_1) {
		b2Vec2 pw = g_camera.ConvertScreenToWorld(ps);
		if (action == GLFW_PRESS) {
			app->MouseDown(pw);
		}
		if (action == GLFW_RELEASE) {app->MouseUp(pw);}
	}
	else if (button == GLFW_MOUSE_BUTTON_2)	{
		if (action == GLFW_PRESS) {
			lastp = g_camera.ConvertScreenToWorld(ps);
			rightMouseDown = true;
		}
		if (action == GLFW_RELEASE) {rightMouseDown = false;}
	}
}

static void sMouseMotion(GLFWwindow*, double xd, double yd) {
	b2Vec2 ps((float)xd, (float)yd);
	b2Vec2 pw = g_camera.ConvertScreenToWorld(ps);
	app->MouseMove(pw);
	if (rightMouseDown)	{
		b2Vec2 diff = pw - lastp;
		g_camera.m_center.x -= diff.x;
		g_camera.m_center.y -= diff.y;
		lastp = g_camera.ConvertScreenToWorld(ps);
	}
}

static void sScrollCallback(GLFWwindow* window, double dx, double dy) {
	ImGui_ImplGlfwGL3_ScrollCallback(window, dx, dy);
	bool mouse_for_ui = ImGui::GetIO().WantCaptureMouse;
	if (!mouse_for_ui) {
		if (dy > 0) {g_camera.m_zoom /= 1.1f;}
		else {g_camera.m_zoom *= 1.1f;}
	}
}

static void sRestart() {
	delete app;
	app = new Application;
}

static void sSimulate() {
	glEnable(GL_DEPTH_TEST);
	app->Step(&settings);
	app->DrawTitle("Controles : Esc=quitter, fleches ou souris+droit=deplacer, souris+gauche=selectionner+deplacer, haut/bas=zoom, R=reset, space=balle, P=pause");
	glDisable(GL_DEPTH_TEST);

}

static void sInterface() {
	int menuWidth = 300;
	if (ui.showMenu) {
		ImGui::SetNextWindowPos(ImVec2((float)g_camera.m_width - menuWidth - 10, 10));
		ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)g_camera.m_height - 20));
		ImGui::Begin("Proprietes de l'application", &ui.showMenu, ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoCollapse);
		ImGui::PushAllowKeyboardFocus(false); // Disable TAB

		ImGui::Text("Scene");
		ImGui::Separator();
		ImGui::Checkbox("Rotation plateforme", &settings.rotationPlateforme);
		ImGui::Checkbox("Active balles", &settings.launchBalls);
		ImGui::Checkbox("Ajoute objet", &settings.launchObject);
		// ImGui::SliderFloat("##Gain", $settings.nb_balls, 0.0f, 500.0f, "%.0f");

		ImGui::Text("Creature");
		ImGui::Separator();
		ImGui::Checkbox("Centre des masses", &settings.showCOM);
		ImGui::Checkbox("Equilibre", &settings.balanceControl);
		ImGui::Checkbox("Suivi de pose", &settings.motionTracking);
		// Pour ajouter d'autres paramètres à l'interface, ex. gains des régulateurs PD
		// ImGui::SliderFloat("##Gain", s, 0.0f, 500.0f, "%.0f");

		ImGui::Text("Application");
		ImGui::Separator();
		ImGui::Checkbox("Articulations", &settings.drawJoints);
		ImGui::Checkbox("Points de contact", &settings.drawContactPoints);
		ImGui::Checkbox("Normales de contact", &settings.drawContactNormals);

		ImVec2 button_sz = ImVec2(-1, 0);
		//if (ImGui::Button("Pause (P)", button_sz)) settings.pause = !settings.pause;
		if (ImGui::Button("Remise a zero (R)", button_sz)) sRestart();
		if (ImGui::Button("Quitter (Esc)", button_sz)) glfwSetWindowShouldClose(mainWindow, GL_TRUE);

		ImGui::PopAllowKeyboardFocus();
		ImGui::End();
	}
}

void glfwErrorCallback(int error, const char *description) {
	fprintf(stderr, "GLFW error occured. Code: %d. Description: %s\n", error, description);
}

int main(int, char**) {

    // Init GLFW
	glfwSetErrorCallback(glfwErrorCallback);
	if (glfwInit() == 0) {fprintf(stderr, "Failed to initialize GLFW\n");return -1;}

    // Shader version on Linux
#ifndef _WIN32
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // _WIN32

	// Window
	char title[64];
	sprintf(title, "M1IF37 TP CONTROLEUR");
	mainWindow = glfwCreateWindow(g_camera.m_width, g_camera.m_height, title, NULL, NULL);
	if (mainWindow == NULL) {fprintf(stderr, "Failed to open GLFW mainWindow.\n");glfwTerminate();return -1;}

	// Callbacks
	glfwMakeContextCurrent(mainWindow);
	glfwSetScrollCallback(mainWindow, sScrollCallback);
	glfwSetWindowSizeCallback(mainWindow, sResizeWindow);
	glfwSetKeyCallback(mainWindow, sKeyCallback);
	glfwSetCharCallback(mainWindow, sCharCallback);
	glfwSetMouseButtonCallback(mainWindow, sMouseButton);
	glfwSetCursorPosCallback(mainWindow, sMouseMotion);
	glfwSetScrollCallback(mainWindow, sScrollCallback);

	// Init GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)	{fprintf(stderr, "Error: %s\n", glewGetErrorString(err));exit(EXIT_FAILURE);}
	g_debugDraw.Create();
	sCreateUI(mainWindow);

	// Create application
	app = new Application;

	// Control the frame rate. One draw per monitor refresh.
	glfwSwapInterval(1);
	glClearColor(0.0f, 0.0f, 0.0f, 1.f);

	while (!glfwWindowShouldClose(mainWindow)) {
        // window
		glfwGetWindowSize(mainWindow, &g_camera.m_width, &g_camera.m_height);
        int bufferWidth, bufferHeight;
        glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
        glViewport(0, 0, bufferWidth, bufferHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // GUI
		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::SetNextWindowPos(ImVec2(0,0));
		ImGui::SetNextWindowSize(ImVec2((float)g_camera.m_width, (float)g_camera.m_height));
		ImGui::Begin("Overlay", NULL, ImVec2(0,0), 0.0f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoInputs|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoScrollbar);
		ImGui::SetCursorPos(ImVec2(5, (float)g_camera.m_height - 20));
		ImGui::End();

        // Run time step
		sSimulate();
		sInterface();

		// Render and poll events
		ImGui::Render();
		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}

    // Delete
	if (app) {
		delete app;
		app = NULL;
	}
	g_debugDraw.Destroy();
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();

	return 0;
}
