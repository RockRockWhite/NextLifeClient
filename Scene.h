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
#include <Urho3D/Graphics/Animation.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/AnimationState.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/IO/File.h>
#include <Urho3D/IO/FileSystem.h>


using namespace Urho3D;
class scene
{
public:
	scene(Context* context, ResourceCache* cache, Renderer* render, FileSystem* filesystem)
	{
		//Free
		context_ = context;
		cache_ = cache;
		render_ = render;
		filesystem_ = filesystem;
		CreateScene();
		SetupViewport();
	}
	SharedPtr <Node> GetCameraNode()
	{
		//Get CameraNode;
		return CameraNode;
	}
	SharedPtr <Scene> GetScene()
	{
		//Get CameraNode;
		return scene_;
	}
	void Update(float timeStep)
	{
		const float MODEL_MOVE_SPEED = 2.0f;
		const float MODEL_ROTATE_SPEED = 100.0f;
		//AddTime
		for (int i = 0; i < NUM_MODELS; i++)
		{
			//Get node
			SharedPtr<Node> ModelNode;
			ModelNode = scene_->GetChild("Model" +(String)i);
			AnimatedModel* ModelObject= ModelNode->GetComponent<AnimatedModel>();
			AnimationState* WalkState = ModelObject->GetAnimationState((unsigned)0);

			//Walk forword
			ModelNode->Translate(Vector3::FORWARD*MODEL_MOVE_SPEED*timeStep);
			//Turn.
			Vector3 pos = ModelNode->GetPosition();

			if (pos.x_ < -47 || pos.x_ >47 || pos.z_ < -47|| pos.z_ >47)
			{
				ModelNode->Yaw(MODEL_ROTATE_SPEED*timeStep);
			}
			WalkState->AddTime(timeStep);
		}
	}
private:
	void CreateScene()
	{
		scene_ = new Scene(context_);

		//Set quality
		render_->SetShadowMapSize(10240);
		render_->SetMaterialQuality(QUALITY_MAX);

		//Create Octree
		scene_->CreateComponent<Octree>();
		scene_->CreateComponent<PhysicsWorld>();
		scene_->CreateComponent<DebugRenderer>();

		File loadFile(context_, filesystem_->GetProgramDir() + "Data/Scenes/Moutain.xml", FILE_READ);
		scene_->LoadXML(loadFile);
		//Create Zone
		ZoneNode = scene_->CreateChild("Zone");
		zone = ZoneNode->CreateComponent<Zone>();
		zone->SetAmbientColor(Color(0.5f, 0.5f, 0.5f));

		//Create Skybox
		SkyNode = scene_->CreateChild("Sky");
		skybox = SkyNode->CreateComponent<Skybox>();
		skybox->SetModel(cache_->GetResource<Model>("Models/Box.mdl"));
		skybox->SetMaterial(cache_->GetResource<Material>("Materials/Skybox.xml"));

		//Create Plane
		PlaneNode = scene_->CreateChild("Plane");
		PlaneNode->SetPosition(Vector3(0.0f, -0.5f, 0.0f));
		PlaneNode->SetScale(Vector3(500.0f, 1.0f, 500.0f));
		plane = PlaneNode->CreateComponent<StaticModel>();
		plane->SetModel(cache_->GetResource<Model>("Models/Box.mdl"));
		plane->SetMaterial(cache_->GetResource<Material>("Materials/StoneTiled.xml"));
		//Set physics
		auto* PlaneBody=PlaneNode->CreateComponent<RigidBody>();
		PlaneBody->SetCollisionLayer(2);
		CollisionShape* PlaneShape=PlaneNode->CreateComponent<CollisionShape>();
		PlaneShape->SetBox(Vector3::ONE);


		//Create Light
		LightNode = scene_->CreateChild("Light");
		LightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));
		light = LightNode->CreateComponent<Light>();
		light->SetLightType(LIGHT_DIRECTIONAL);
		light->SetCastShadows(true);

		//Create Camera
		CameraNode = scene_->CreateChild("Camera");
		CameraNode->CreateComponent<Camera>();
		CameraNode->SetPosition(Vector3(0.0f, 5.0f, 0.0f));

		//Create Models
		for (int i = 0; i < NUM_MODELS; i++)
		{
			SharedPtr<Node> ModelNode;
			ModelNode = scene_->CreateChild("Model" + (String)i);
			ModelNode->SetPosition(Vector3(Random(90.0f) - 45.0f, 0.0f, Random(90.0f) - 45.0f));
			ModelNode->SetRotation(Quaternion(0.0f, Random(360.0f), 0.0f));
			AnimatedModel* ModelObject = ModelNode->CreateComponent<AnimatedModel>();
			ModelObject->SetModel(cache_->GetResource<Model>("Models/Kachujin/Kachujin.mdl"));
			ModelObject->SetMaterial(cache_->GetResource<Material>("Models/Kachujin/Materials/Kachujin.xml"));
			//Shadows
			ModelObject->SetCastShadows(true);

			
			AnimationState* WalkAState = ModelObject->AddAnimationState(cache_->GetResource<Animation>("Models/Kachujin/Kachujin_Walk.ani"));
			if (WalkAState)
			{
				WalkAState->SetWeight(1.0f);
				WalkAState->SetLooped(true);
				WalkAState->SetTime(WalkAState->GetAnimation()->GetLength());
			}
		}

		//Create Boxs;
		for (int i = 0; i <1000; i++)
		{
			auto* BoxNode = scene_->CreateChild("Box");
			SharedPtr<StaticModel> BoxObject;
			BoxNode->SetPosition(Vector3(0, 2.0f * i+100, 0));
			BoxObject = BoxNode->CreateComponent<StaticModel>();
			BoxObject->SetModel(cache_->GetResource<Model>("Models/Box.mdl"));
			BoxObject->SetMaterial(cache_->GetResource<Material>("Materials/StoneSmall.xml"));
			BoxObject->SetCastShadows(true);

			//Physics
			auto* BoxRigidBody = BoxNode->CreateComponent<RigidBody>();
			BoxRigidBody->SetMass(1.0f);
			BoxRigidBody->SetFriction(1.0f);
			BoxRigidBody->SetCollisionEventMode(COLLISION_NEVER);
			auto* BoxCollisionShape = BoxNode->CreateComponent<CollisionShape>();
			BoxCollisionShape->SetBox(Vector3::ONE);
		}

		/***********************************************************************
		CreateResources
		***********************************************************************/
		const int NUM_MUSHROOMS = 2000;

		//Create mushrooms
		
		for(int i = 0; i < NUM_MUSHROOMS; i++)
		{
			SharedPtr<Node> MushroomNode;
			MushroomNode=scene_->CreateChild("mushroom");
			MushroomNode->SetPosition(Vector3(Random(250.0f) -250.0f, 0.0f, Random(250.0f) - 250.0f));
			MushroomNode->SetScale(Random(0.2f,0.8f));
			MushroomNode->SetRotation(Quaternion(0.0f, Random(360.0f), 0.0f));
			SharedPtr<StaticModel> MushroomObject;
			MushroomObject = MushroomNode->CreateComponent<StaticModel>();
			MushroomObject->SetModel(cache_->GetResource<Model>("Models/Mushroom.mdl"));
			MushroomObject->SetMaterial(cache_->GetResource<Material>("Materials/Mushroom.xml"));
			MushroomObject->SetCastShadows(true);
			
			RigidBody* MushroomRigidBody = MushroomNode->CreateComponent<RigidBody>();
			MushroomRigidBody->SetCollisionLayer(4);

			CollisionShape* MushroomCollisionShape = MushroomNode->CreateComponent<CollisionShape>();
			MushroomCollisionShape->SetTriangleMesh(MushroomObject->GetModel(), 0);
			
		}



	}
	void SetupViewport()
	{
		SharedPtr<Viewport> viewport_(new Viewport(context_, scene_, CameraNode->GetComponent<Camera>()));
		render_->SetViewport(0, viewport_);
	}
	const unsigned NUM_MODELS = 100;
	Context* context_;
	ResourceCache* cache_;
	Renderer* render_;
	FileSystem* filesystem_;
	SharedPtr <Scene> scene_;
	SharedPtr <Node> ZoneNode;
	SharedPtr <Node> SkyNode;
	SharedPtr <Node> CameraNode;
	SharedPtr <Node> PlaneNode;
	SharedPtr <Node> LightNode;

	Skybox* skybox;
	StaticModel* plane;
	Light* light;
	Zone* zone;
};
