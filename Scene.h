#pragma once

#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/Node.h>

#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Graphics/Material.h>

#include <Urho3D/Resource/ResourceCache.h>

using namespace Urho3D;
class scene
{
public:
	scene(Context* context,ResourceCache* cache)
	{
		scene_ = new Scene(context);
		cache_ = cache;
		skyNode = scene_->CreateChild("Sky");
		skybox = skyNode->CreateComponent<Skybox>();
		skybox->SetModel(cache_->GetResource<Model>("Models/Box.mdl"));
		skybox->SetMaterial(cache->GetResource<Material>("Materials/Skybox.xml"));
	}

private:
	SharedPtr<Scene> scene_;
	SharedPtr<Node> skyNode;
	Skybox* skybox;
	ResourceCache* cache_;
};
