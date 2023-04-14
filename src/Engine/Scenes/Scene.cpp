#pragma once

#include "Scene.h"

//TODO: Implement gang of three
Scene::Scene(SceneManager *p_sceneManager) : m_sceneManager(p_sceneManager)
{
	m_entityManager = new EntityManager();
	m_systemManager = new SystemManager();
}

Scene::~Scene() = default;

/// Original Author: Piotr Moskala
/// <summary>
/// Clears any common scene elements
/// </summary>
void Scene::Close()
{
	delete m_entityManager;
	delete m_systemManager;
}