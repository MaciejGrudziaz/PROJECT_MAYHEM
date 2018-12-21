#include "Graphics.h"

BasicShaderStruct Graphics::ground;
BasicShaderStruct Graphics::axis;

void Graphics::Init() {
	WinAPIwindowManager::RegisterWindow(new WinAPIwindow("MAIN", 1280, 720));

	VSync(0);

	//Camera* camera = new Camera(glm::vec3(0.0f, 7.5f, 20.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	InitShaders();

	//new Shader("Ground", "SHADERS/groundShader.vert", "SHADERS/groundShader.frag");
	//new Shader("Axis", "SHADERS/axisShader.vert", "SHADERS/axisShader.frag");
	//new Shader("DynamicObj", "SHADERS/shader1_Dynamic.vert", "SHADERS/shader1_Dynamic.frag");
	//new Shader("StaticObj", "SHADERS/shader1_Static.vert", "SHADERS/shader1_Static.frag");
	//new Shader("DynamicHitbox", "SHADERS/shader2_Dynamic.vert", "SHADERS/shader2_Dynamic.frag");
	//new Shader("StaticHitbox", "SHADERS/shader2_Static.vert", "SHADERS/shader2_Static.frag");
	//new Shader("HitboxCompute", "SHADERS/hitboxComputeShader.comp");

	LoadBasicShaderStructs();

	//LoadModel_GL("MODELS/TEST.mgr");
	//AutoList<Character>::GetObj(0)->StartAnimation(0);

	//ResourcesManager::InitTimer();

	glEnable(GL_DEPTH_TEST);
}

void Graphics::Process() {
	//ResourcesManager::StopFrameTimeMeasurment();
	//ResourcesManager::StartFrameTimeMeasurment();

	//int frameCount = ResourcesManager::GetFrameCount();

	//for (unsigned i = 0; i < frameCount; ++i) {
	//	for (unsigned i = 0; i < AutoList<Character>::GetCount(); ++i)
	//		AutoList<Character>::GetObj(i)->Update();
	//}

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//AutoList<Camera>::GetObj(0)->Process();

	Camera* cam=nullptr;

	for (int i = 0; i < AutoList<Camera>::GetCount(); ++i) {
		if (AutoList<Camera>::GetObj(i)->IsActive()) {
			cam = AutoList<Camera>::GetObj(i);
			break;
		}
	}

	cam->Update();

	for (unsigned i = 0; i < AutoList<Character>::GetCount(); ++i) {
		AutoList<Character>::GetObj(i)->SetViewMatrix(cam->GetView());
		AutoList<Character>::GetObj(i)->Draw();
	}

	//if (ground.render == true) DrawGround();
	//if (axis.render == true) {
	//	glDisable(GL_DEPTH_TEST);
	//	DrawAxis();
	//	glEnable(GL_DEPTH_TEST);
	//}
}

void Graphics::End() {

}

//-----------------------------------------------------------------------------
//-----------------------------------PRIVATE-----------------------------------
//-----------------------------------------------------------------------------

void Graphics::DrawAxis() {
	glUseProgram(axis.shaderProgram);
	glUniformMatrix4fv(axis.viewMatLoc, 1, GL_FALSE, glm::value_ptr(AutoList<Camera>::GetObj(0)->GetView()));
	glDrawArrays(GL_LINES, 0, 6);
}

void Graphics::DrawGround() {
	glUseProgram(ground.shaderProgram);
	glUniformMatrix4fv(ground.viewMatLoc, 1, GL_FALSE, glm::value_ptr(AutoList<Camera>::GetObj(0)->GetView()));
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Graphics::LoadBasicShaderStructs() {
	ground.shaderProgram = AutoMap<Shader>::GetObj("Ground")->GetProgram();
	axis.shaderProgram = AutoMap<Shader>::GetObj("Axis")->GetProgram();

	ground.projectionMatLoc = glGetUniformLocation(ground.shaderProgram, "projection");
	ground.viewMatLoc = glGetUniformLocation(ground.shaderProgram, "view");

	axis.projectionMatLoc = glGetUniformLocation(axis.shaderProgram, "projection");
	axis.viewMatLoc = glGetUniformLocation(axis.shaderProgram, "view");

	glUseProgram(ground.shaderProgram);
	glUniformMatrix4fv(ground.projectionMatLoc, 1, GL_FALSE, glm::value_ptr(WinAPIwindowManager::GetMainWindow()->GetProjectionMatrix()));
	glUseProgram(axis.shaderProgram);
	glUniformMatrix4fv(axis.projectionMatLoc, 1, GL_FALSE, glm::value_ptr(WinAPIwindowManager::GetMainWindow()->GetProjectionMatrix()));
}

bool Graphics::VSync(int on_off) {
	PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;

	_wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");

	if (_wglGetExtensionsStringEXT != nullptr) {
		if (strstr(_wglGetExtensionsStringEXT(), "WGL_EXT_swap_control") != nullptr) {
			PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT = NULL;
			PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT = NULL;

			wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
			wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");

			if (wglSwapIntervalEXT != nullptr) {
				if (on_off == 0 || on_off == 1) {
					wglSwapIntervalEXT(on_off);
					return true;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}
	else return false;
}

void Graphics::InitShaders() {
	new Shader("Ground", "SHADERS/groundShader.vert", "SHADERS/groundShader.frag");
	new Shader("Axis", "SHADERS/axisShader.vert", "SHADERS/axisShader.frag");
	new Shader("DynamicObj", "SHADERS/shader1_Dynamic.vert", "SHADERS/shader1_Dynamic.frag");
	new Shader("StaticObj", "SHADERS/shader1_Static.vert", "SHADERS/shader1_Static.frag");
	new Shader("DynamicHitbox", "SHADERS/shader2_Dynamic.vert", "SHADERS/shader2_Dynamic.frag");
	new Shader("StaticHitbox", "SHADERS/shader2_Static.vert", "SHADERS/shader2_Static.frag");
	new Shader("DynamicHitboxCompute", "SHADERS/hitboxComputeShader_Dynamic.comp");
	new Shader("StaticHitboxCompute", "SHADERS/hitboxComputeShader_Static.comp");
}
