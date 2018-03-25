#include "Referee.h"
#include "Common/misc/Cgdi.h"
#include "SteeringBehaviors.h"
#include "SoccerTeam.h"
#include "SoccerPitch.h"
#include "Common/2D/transformations.h"
#include "RefereeStates.h"
#include "Goal.h"
#include "Common/game/EntityFunctionTemplates.h"
#include "AIParamLoader.h"
#include "GlobalParamLoader.h"



//----------------------------- ctor ------------------------------------
//-----------------------------------------------------------------------
Referee::Referee(State<Referee>* start_state,
	Vector2D           heading,
	Vector2D           velocity,
	double              mass,
	double              max_force,
	double              max_speed,
	double              max_turn_rate,
	double              scale,
	Vector2D			 startLocation,
	SoccerPitch*		pitch) : PlayerBase(pitch,
		heading,
		velocity,
		mass,
		max_force,
		max_speed,
		max_turn_rate,
		scale,
		referee,
		startLocation
		),
	m_vStartLoc(startLocation)


{
	//set up the state machine
	m_pStateMachine = new StateMachine<Referee>(this);

	m_pStateMachine->SetCurrentState(start_state);
	m_pStateMachine->SetPreviousState(start_state);
	m_pStateMachine->SetGlobalState(GlobalRefereeState::Instance());

	m_pStateMachine->CurrentState()->Enter(this);
}



//-------------------------- Update --------------------------------------

void Referee::Update()
{
	//run the logic for the current state
	m_pStateMachine->Update();

	//calculate the combined force from each steering behavior 
	Vector2D SteeringForce = m_pSteering->Calculate();



	//Acceleration = Force/Mass
	Vector2D Acceleration = SteeringForce / m_dMass;

	//update velocity
	m_vVelocity += Acceleration;

	//make sure player does not exceed maximum velocity
	m_vVelocity.Truncate(m_dMaxSpeed);

	//update the position
	m_vPosition += m_vVelocity;


	//enforce a non-penetration constraint if desired
	if (Prm.bNonPenetrationConstraint)
	{
		EnforceNonPenetrationContraint(this, AutoList<PlayerBase>::GetAllMembers());
	}

	//update the heading if the player has a non zero velocity
	if (!m_vVelocity.isZero())
	{
		m_vHeading = Vec2DNormalize(m_vVelocity);

		m_vSide = m_vHeading.Perp();
	}

	m_vLookAt = Vec2DNormalize(Ball()->Pos() - Pos());
}


//-------------------- HandleMessage -------------------------------------
//
//  routes any messages appropriately
//------------------------------------------------------------------------
bool Referee::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

//--------------------------- Render -------------------------------------
//
//------------------------------------------------------------------------
void Referee::Render()
{
	gdi->GreenPen();

	m_vecPlayerVBTrans = WorldTransform(m_vecPlayerVB,
		Pos(),
		m_vLookAt,
		m_vLookAt.Perp(),
		Scale());

	gdi->ClosedShape(m_vecPlayerVBTrans);

	//draw the head
	gdi->BrownBrush();
	gdi->Circle(Pos(), 6);

	//draw the ID
	if (Prm.bIDs)
	{
		gdi->TextColor(0, 170, 0);;
		gdi->TextAtPos(Pos().x - 20, Pos().y - 20, ttos(ID()));
	}

	//draw the state
	if (Prm.bStates)
	{
		gdi->TextColor(0, 170, 0);
		gdi->TransparentText();
		gdi->TextAtPos(m_vPosition.x, m_vPosition.y - 20, std::string(m_pStateMachine->GetNameOfCurrentState()));
	}
}