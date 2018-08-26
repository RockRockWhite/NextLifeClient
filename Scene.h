#pragma once
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Renderer.h>
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

		//Create Camera
		CameraNode = scene_->CreateChild("Camera");
		CameraNode->CreateComponent<Camera>();
	
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
	Skybox* skybox;

};
