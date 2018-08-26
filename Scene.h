#pragma once
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Resource/ResourceCache.h>

using namespace Urho3D;
class scene
{
public:
	scene(Context* context, ResourceCache* cache, Renderer* render)
	{
		context_ = context;
		cache_ = cache;
		render_ = render;

		CreateScene();
		SetupViewport();
	}

private:
	void CreateScene()
	{
		scene_ = new Scene(context_);

		//Create Octree
		scene_->CreateComponent<Octree>();

		//Create Skybox
		SkyNode = scene_->CreateChild("Sky");
		skybox = SkyNode->CreateComponent<Skybox>();
		skybox->SetModel(cache_->GetResource<Model>("Models/Box.mdl"));
		skybox->SetMaterial(cache_->GetResource<Material>("Materials/Skybox.xml"));

		//Create Plane
		PlaneNode = scene_->CreateChild("Plane");
		PlaneNode->SetScale(Vector3(100.0f, 10.0f, 100.0f));
		plane = PlaneNode->CreateComponent<StaticModel>();
		plane->SetModel(cache_->GetResource<Model>("Models/Plane.mdl"));
		plane->SetMaterial(cache_->GetResource<Material>("Materials/StoneTiled.xml"));

		//Create Light
		LightNode = scene_->CreateChild("Light");
		LightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));
		light=LightNode->CreateComponent<Light>();
		light->SetLightType(LIGHT_DIRECTIONAL);

		//Create Camera
		CameraNode = scene_->CreateChild("Camera");
		CameraNode->CreateComponent<Camera>();
		CameraNode->SetPosition(Vector3(0.0f, 5.0f, 0.0f));
	}

	void SetupViewport()
	{
		SharedPtr<Viewport> viewport_(new Viewport(context_, scene_, CameraNode->GetComponent<Camera>()));
		render_->SetViewport(0, viewport_);
	}
	Context* context_;
	ResourceCache* cache_;
	Renderer* render_;
	SharedPtr <Scene> scene_;
	SharedPtr <Node> SkyNode;
	SharedPtr <Node> CameraNode;
	SharedPtr <Node> PlaneNode;
	SharedPtr <Node> LightNode;

	Skybox* skybox;
	StaticModel* plane;
	Light* light;

};
