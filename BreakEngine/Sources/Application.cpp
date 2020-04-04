#include "pch.h"
#include "Application.h"

void Application::Initialize() {
	m_pFileManager = new FileManager();
	m_pResourceManager = new ResourceManager();

	m_pFileManager->Initialize();
	m_pResourceManager->Initialize(m_pFileManager);
}
void Application::Terminate() {
	m_pResourceManager->Terminate();
	m_pFileManager->Terminate();
}