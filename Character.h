#pragma once
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/Text.h>
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

const float MOVE_SPEED = 10.0f;
const float MOUSE_SENSITIVITY=0.1f;
const float GROUND_FORCE = 0.8f;
const float LAY_DISTANCE = 5.0f;

class Character
{
public:
	Character(Context* context, SharedPtr <Scene> scene, SharedPtr <Node> camera, UIElement* UIRoot,ResourceCache* cache)
	{
		context_ = context;
		scene_ = scene;
		cache_= cache;
		camera_ = camera;
		root_ = UIRoot;
		CreateCharacter();
	}
	void Update(float timeStep, Controls* controls)
	{
		//Set Roatation
		CharacterNode->SetRotation(Quaternion(controls->yaw_, Vector3::UP));

		Vector3 moveDir = Vector3::ZERO;
		Quaternion rot = CharacterNode->GetRotation();
		Vector3 pos = CharacterNode->GetPosition();
		auto CharacterBody = CharacterNode->GetComponent<RigidBody>();

		//Apply Force
		Vector3 LineVelocity = CharacterBody->GetLinearVelocity();
		Vector3 PlaneVelocity = Vector3(LineVelocity.x_, 0.0f, LineVelocity.z_);
		CharacterBody->ApplyImpulse(-PlaneVelocity*GROUND_FORCE);

		//Get controls
		if (controls->IsDown(CTRL_FORWARD))
			moveDir += Vector3::FORWARD;
		if (controls->IsDown(CTRL_BACK))
			moveDir += Vector3::BACK;
		if (controls->IsDown(CTRL_LEFT))
			moveDir += Vector3::LEFT;
		if (controls->IsDown(CTRL_RIGHT))
			moveDir += Vector3::RIGHT;

		//Normalize
		if (moveDir.LengthSquared() > 0.0f)
			moveDir.Normalize();

		//Walk
		CharacterBody->ApplyImpulse(rot*moveDir*MOVE_SPEED*GROUND_FORCE);

		//Limit pitch
		controls->pitch_ = Clamp(controls->pitch_, -80.0f, 80.0f);

		//Set Camera
		Quaternion dir = rot*Quaternion(controls->pitch_, Vector3::RIGHT);
		camera_->SetRotation(dir);

		Vector3 RayDir = dir*Vector3::BACK;
		Vector3 origin = pos + rot*Vector3(0.0f, 1.7f, 0.0f);

		//Raycast
		float distance = LAY_DISTANCE;
		PhysicsRaycastResult result;
		scene_->GetComponent<PhysicsWorld>()->RaycastSingle(result, Ray(origin, RayDir), distance, 2);
		if (result.body_)
			distance = result.distance_;

		camera_->SetPosition(origin + RayDir * distance);


		//Play Animations
			
		if (!moveDir.Equals(Vector3::ZERO))
			AnimCtrl->PlayExclusive("Models/Mutant/Mutant_Run.ani", 0, true, 0.2f);
		else
			AnimCtrl->PlayExclusive("Models/Mutant/Mutant_Idle0.ani", 0, true, 0.2f);

		/***********************************************************************
		PickUpdate
		***********************************************************************/
		PhysicsRaycastResult PickResult;
		//scene_->GetComponent<PhysicsWorld>()->RaycastSingle(PickResult, Ray(camera_->GetPosition(), camera_->GetRotation()*Vector3::ONE),100,4);
		

		scene_->GetComponent<PhysicsWorld>()->SphereCast(PickResult, Ray(CharacterNode->GetPosition(), CharacterNode->GetRotation()*Vector3(1,0,1)),2,2,4);
		
		auto* PickText = root_->GetChildStaticCast<Text>("PickText", true);
		if (PickResult.body_)
		{
			//GetPickNode
			auto* PickNode = PickResult.body_->GetNode();
			PickText->SetVisible(true);
			PickText->SetText("Press F to pick the " + PickNode->GetName());

			//Remove Node
			if (controls->IsDown(CTRL_PICK))
			{
				PickNode->Remove();
			}
		}
		else
		{
			PickText->SetVisible(false);
		}

	}


private:
	Context* context_;
	ResourceCache* cache_;
	SharedPtr <Scene> scene_;
	SharedPtr <Node> camera_;
	SharedPtr<Node> CharacterNode;
	AnimatedModel* CharacterObject;
	AnimationController* AnimCtrl;
	UIElement* root_;

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
		CharacterBody->SetMass(50.0f);
		CharacterBody->SetPosition(Vector3(0, 100, 0));

		CharacterBody->SetAngularFactor(Vector3::ZERO);
		auto* CharacterShape=CharacterNode->CreateComponent<CollisionShape>();
		CharacterShape->SetCapsule(0.7f, 1.8f, Vector3(0.0f, 0.9f, 0.0f));
	}

};

