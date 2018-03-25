#include "RefereeStates.h"
#include "Common/Debug/DebugConsole.h"
#include "SoccerPitch.h"
#include "PlayerBase.h"
#include "Referee.h"
#include "SteeringBehaviors.h"
#include "SoccerTeam.h"
#include "Goal.h"
#include "Common/2D/geometry.h"
#include "FieldPlayer.h"
#include "AIParamLoader.h"
#include "GlobalParamLoader.h"
#include "Common/Messaging/Telegram.h"
#include "Common/Messaging/MessageDispatcher.h"
#include "SoccerMessages.h"



//--------------------------- GlobalKeeperState -------------------------------
//-----------------------------------------------------------------------------

GlobalRefereeState* GlobalRefereeState::Instance()
{
	static GlobalRefereeState instance;

	return &instance;
}
void GlobalRefereeState::Enter(Referee* ref)
{

	ref->GetFSM()->ChangeState(RotateChaseBall::Instance());
}


//-------------------------RotateChaseBall : ----------------------------------
//
//  In this state the ref rotates around the ball 
//------------------------------------------------------------------------

RotateChaseBall* RotateChaseBall::Instance()
{
	static RotateChaseBall instance;

	return &instance;
}


void RotateChaseBall::Enter(Referee* ref)
{

	ref->Steering()->ArriveOn();
}

void RotateChaseBall::Execute(Referee* ref)
{
	if (!ref->Pitch()->GameOn())
	{
		ref->GetFSM()->ChangeState(ReturnToCenter::Instance());

	}


	//Rotates around the ball at a 100 radius
	Vector2D ballPos = ref->Pitch()->Ball()->Pos();

	static const double radius = 100.0f;
	//vecttor to ball from ref
	Vector2D vecRefBall = ref->Pos() - ballPos;
	double length = vecRefBall.Length();
	//stays around the radius
	Vector2D vecRadiusPos = ballPos + vecRefBall / length * radius;



	ref->Steering()->SetTarget(vecRadiusPos);
	ref->Steering()->ArriveOn();

}

void RotateChaseBall::Exit(Referee* ref)
{
	ref->Steering()->ArriveOff();
}


void rotateAround()
{

}


//------------------------- ReturnHome: ----------------------------------
//
//  In this state the ref simply returns back to the center of
//  the pitch
//------------------------------------------------------------------------

ReturnToCenter* ReturnToCenter::Instance()
{
	static ReturnToCenter instance;

	return &instance;
}


void ReturnToCenter::Enter(Referee* ref)
{
	ref->Steering()->ArriveOn();
}

void ReturnToCenter::Execute(Referee* ref)
{
	if (ref->Pitch()->GameOn())
	{
		ref->GetFSM()->ChangeState(RotateChaseBall::Instance());

	}

	ref->Steering()->SetTarget(ref->HomeRegion());

}

void ReturnToCenter::Exit(Referee* ref)
{
	ref->Steering()->ArriveOff();
}


