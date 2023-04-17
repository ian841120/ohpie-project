#pragma once
#include <vector>
#include "Light.h"
class LightManager
{
private:
	LightManager() {};
	~LightManager();
public:
	//singleton
	static LightManager& Instance()
	{
		static LightManager lightManager;
		return lightManager;
	}
	void Remove(Light* light);
	// register the light
	void Register(Light* light);
	void PushRenderContext(RenderContext& rc);
	//clear all light
	void Clear();
	void DrawDebugGUI();
	void DrawDebugPrimitive();
private:
	std::vector<Light*>lights;
};