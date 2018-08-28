#pragma once
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/AnimationState.h>
#include <Urho3D/Graphics/Animation.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Input/Controls.h>

#include "Controls_Def.h"

using namespace Urho3D;
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

		const float MOVE_SPEED =20.0f;
		const float MOUSE_SENSITIVITY = 0.1f;



		Vector3 moveDir = Vector3::ZERO;
		if (controls->IsDown(CTRL_FORWARD))
			moveDir += Vector3::FORWARD;
		if (controls->IsDown(CTRL_BACK))
			moveDir += Vector3::BACK;
		if (controls->IsDown(CTRL_LEFT))
			moveDir += Vector3::LEFT;
		if (controls->IsDown(CTRL_RIGHT))
			moveDir += Vector3::RIGHT;
		
		moveDir.Normalize();







		RigidBody *b=CharacterNode->GetComponent<RigidBody>();

		CharacterNode->SetRotation(Quaternion(0.0f, controls->yaw_ - 180.0f, 0.0f));

		b->ApplyImpulse(Quaternion(0.0f, controls->yaw_ , 0.0f)*moveDir*MOVE_SPEED*0.8f);
		
		b->ApplyImpulse(-Vector3(b->GetLinearVelocity().x_, 0.00f, b->GetLinearVelocity().z_)*0.8f);
		
		camera_->SetRotation(Quaternion(0.0f, controls->yaw_ , 0.0f));
		
		Vector3 pos = HeadNode->GetWorldPosition();
		camera_->SetPosition(pos + Vector3::BACK * 5);
	}


private:
	Context* context_;
	ResourceCache* cache_;
	SharedPtr <Scene> scene_;
	SharedPtr <Node> camera_;
	SharedPtr<Node> CharacterNode;
	SharedPtr<Node> HeadNode;
	AnimatedModel* CharacterObject;
	void CreateCharacter()
	{
		//Create Character Node
		CharacterNode = scene_->CreateChild("Bill");
		//Set Rotation
		CharacterNode->SetRotation(Quaternion(0, 180.0f, 0));
		CharacterObject = CharacterNode->CreateComponent<AnimatedModel>();
		CharacterObject->SetModel(cache_->GetResource<Model>("Models/Mutant/Mutant.mdl"));
		CharacterObject->SetMaterial(cache_->GetResource<Material>("Models/Mutant/Materials/mutant_M.xml"));
		CharacterObject->SetCastShadows(true);
		
		//Get Head Node.
		HeadNode=CharacterNode->GetChild("Mutant:Head", true);

		//Init physics;
		auto* CharacterBody=CharacterNode->CreateComponent<RigidBody>();
		CharacterBody->SetMass(1.0f);
		CharacterBody->SetPosition(Vector3(0, 20, 0));
		CharacterBody->SetCollisionLayer(1);
		CharacterBody->SetCollisionEventMode(COLLISION_ALWAYS);
		CharacterBody->SetAngularFactor(Vector3::ZERO);

		auto* CharacterShape=CharacterNode->CreateComponent<CollisionShape>();
		CharacterShape->SetCapsule(0.7f, 1.8f, Vector3(0.0f, 0.9f, 0.0f));
		
	}

};

