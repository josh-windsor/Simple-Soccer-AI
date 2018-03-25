#ifndef REFEREE_H
#define REFEREE_H
//------------------------------------------------------------------------
//
//  Name:   GoalKeeper.h
//
//  Desc:   class to implement a goalkeeper agent
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "Common/2D/Vector2D.h"
#include "PlayerBase.h"
#include "Common/FSM/StateMachine.h"

class PlayerBase;




class Referee : public PlayerBase
{
private:

	//an instance of the state machine class
	StateMachine<Referee>*  m_pStateMachine;

	//this vector is updated to point towards the ball and is used when
	//rendering the goalkeeper (instead of the underlaying vehicle's heading)
	//to ensure he always appears to be watching the ball
	Vector2D   m_vLookAt;
	Vector2D   m_vStartLoc;
	Vector2D   m_vRotation;

	float rotAngle = 200.0f;


public:

	Referee(State<Referee>* start_state,
		Vector2D           heading,
		Vector2D           velocity,
		double              mass,
		double              max_force,
		double              max_speed,
		double              max_turn_rate,
		double              scale,
		Vector2D			 startLocation,
		SoccerPitch*		pitch);


	/////////HEHREHREHRHEHREH have just changed params, need to edit states & change 
	// playerbase constructor to take in pitch and use that instead of team for key functs
	~Referee() { delete m_pStateMachine; }

	//these must be implemented
	void        Update();
	void        Render();
	bool        HandleMessage(const Telegram& msg);


	StateMachine<Referee>* GetFSM()const { return m_pStateMachine; }


	Vector2D    LookAt()const { return m_vLookAt; }
	void        SetLookAt(Vector2D v) { m_vLookAt = v; }

	Vector2D    HomeRegion()const { return m_vStartLoc;  }

	float		RotAngle()const { return rotAngle; }
	void		SetRotAngle(float f){ rotAngle = f; }

	Vector2D	Rotation()const { return m_vRotation; }
	void		SetRotation(Vector2D v) { m_vRotation = v; }

};



#endif