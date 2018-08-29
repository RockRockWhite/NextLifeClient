#pragma once
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Graphics/Animation.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Input/Controls.h>

#include "Controls_Def.h"

using namespace Urho3D;

const float MOVE_SPEED = 20.0f;
const float MOUSE_SENSITIVITY = 0.1f;
const float GROUND_FORCE = -0.8f;
const float CAMERA_MIN_DIST = 1.0f;
const float CAMERA_INITIAL_DIST = 5.0f;
const float CAMERA_MAX_DIST = 20.0f;

class Character
{
public:
	Character(Context* context, SharedPtr <Scene> scene, SharedPtr <Node> camera,ResourceCache* cache)
	{
		context_ = context;
		scene_ = scene;
		cache_= cache;
		camera_ = camera;
		CreateCharacter();
	}
	void Update(float timeStep, Controls* controls)
	{
		//Gec Velocity
		auto* CharacterBody = CharacterNode->GetComponent<RigidBody>();
		Vector3 lineVelocity = CharacterBody->GetLinearVelocity();
		Vector3 planeVelocity = Vector3(lineVelocity.x_, 0.0f, lineVelocity.z_);

		//Apply Force
		if (1/*is on ground*/)
		{
			CharacterBody->ApplyImpulse(planeVelocity*GROUND_FORCE);
		}

		//Update controls
		Vector3 moveDir = Vector3::ZERO;
		if (controls->IsDown(CTRL_FORWARD))
			moveDir += Vector3::FORWARD;
		if (controls->IsDown(CTRL_BACK))
			moveDir += Vector3::BACK;
		if (controls->IsDown(CTRL_LEFT))
			moveDir += Vector3::LEFT;
		if (controls->IsDown(CTRL_RIGHT))
			moveDir += Vector3::RIGHT;

		//Normalize move
		if (moveDir.LengthSquared() > 0.0f)
			moveDir.Normalize();

		//		
		CharacterNode->SetRotation(Quaternion(controls->yaw_, Vector3::UP));

		//Get
		Quaternion rot = CharacterNode->GetRotation();

		//Limit pitch
		controls->pitch_= Clamp(controls->pitch_, -80.0f, 80.0f);

		//Set dit
		Quaternion dir = rot*Quaternion(controls->pitch_, Vector3::RIGHT);
		Vector3 RayDir = dir*Vector3::BACK;

		//Move Character
		CharacterBody->ApplyImpulse(rot*moveDir*MOVE_SPEED);



		/*
		float rayDistance = CAMERA_INITIAL_DIST;
		PhysicsRaycastResult result;

		Vector3 origin = CharacterNode->GetPosition() + rot*Vector3(0.0f, 1.7f, 0.0f);
		scene_->GetComponent<PhysicsWorld>()->RaycastSingle(result, Ray(origin, RayDir), rayDistance, 2);

		if (result.body_)
			rayDistance = result.distance_;
			*/

		Vector3 aimPoint = CharacterNode->GetPosition() + rot * Vector3(0.0f, 1.7f, 0.0f);


		float rayDistance = 5;

		PhysicsRaycastResult result;

		scene_->GetComponent<PhysicsWorld>()->RaycastSingle(result, Ray(aimPoint, RayDir), rayDistance, 2);
		if (result.body_)
			rayDistance = result.distance_;

		camera_->SetPosition(aimPoint + RayDir * rayDistance);



		//Set Camera
		//camera_->SetPosition(CharacterNode->GetPosition() + RayDir * rayDistance);
		camera_->SetRotation(dir);



		CharacterNode->GetComponent<AnimationController>(true)->PlayExclusive("Models/Mutant/Mutant_Run.ani", 0, true, 50);
		CharacterNode->GetComponent<AnimationController>(true)->SetSpeed("Models/Mutant/Mutant_Run.ani", 1.0f);

	}


private:
	Context* context_;
	ResourceCache* cache_;
	SharedPtr <Scene> scene_;
	SharedPtr <Node> camera_;
	SharedPtr<Node> CharacterNode;
	AnimatedModel* CharacterObject;
	AnimationController* AnimCtrl;
	void CreateCharacter()
	{
		//Create Character Node
		CharacterNode = scene_->CreateChild("Bill");

		//Adjust to normal state
		auto* AdjustNode = CharacterNode->CreateChild("Adjust");
		AdjustNode->SetRotation(Quaternion(0.0f, 180.0f, 0.0f));

		//Character Model
		CharacterObject = AdjustNode->CreateComponent<AnimatedModel>();

		//Set
		CharacterObject->SetModel(cache_->GetResource<Model>("Models/Mutant/Mutant.mdl"));
		CharacterObject->SetMaterial(cache_->GetResource<Material>("Models/Mutant/Materials/mutant_M.xml"));
		CharacterObject->SetCastShadows(true);

		//Animaition
		AnimCtrl = AdjustNode->CreateComponent<AnimationController>();

		//Init physics;
		auto* CharacterBody=CharacterNode->CreateComponent<RigidBody>();
		CharacterBody->SetMass(1.0f);
		CharacterBody->SetPosition(Vector3(0, 20, 0));
		CharacterBody->SetAngularFactor(Vector3::ZERO);

		auto* CharacterShape=CharacterNode->CreateComponent<CollisionShape>();
		CharacterShape->SetCapsule(0.7f, 1.8f, Vector3(0.0f, 0.9f, 0.0f));
	}

};

