#ifndef REFEREESTATES_H
#define REFEREESTATES_H
//------------------------------------------------------------------------
//
//  Name: GoalKeeperStates.h
//
//  Desc:   Declarations of all the states used by a Simple Soccer
//          goalkeeper
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>
#include "Common/FSM/State.h"
#include "Common/Messaging/Telegram.h"
#include "constants.h"


class Referee;
class SoccerPitch;


class GlobalRefereeState : public State<Referee>
{
private:

	GlobalRefereeState() {}

public:

	//this is a singleton
	static GlobalRefereeState* Instance();

	void Enter(Referee* keeper);

	void Execute(Referee* keeper) {}

	void Exit(Referee* keeper) {}

	bool OnMessage(Referee*, const Telegram&) { return false; }
};

//-----------------------------------------------------------------------------


//------------------------------------------------------------------------

class RotateChaseBall : public State<Referee>
{
private:

	RotateChaseBall() {}

public:

	//this is a singleton
	static RotateChaseBall* Instance();

	void Enter(Referee* keeper);

	void Execute(Referee* keeper);

	void Exit(Referee* keeper);

	bool OnMessage(Referee*, const Telegram&) { return false; }
};

//-----------------------------------------------------------------------------


//------------------------------------------------------------------------
class ReturnToCenter : public State<Referee>
{
private:

	ReturnToCenter() {}

public:

	//this is a singleton
	static ReturnToCenter* Instance();

	void Enter(Referee* keeper);

	void Execute(Referee* keeper);

	void Exit(Referee* keeper);

	bool OnMessage(Referee*, const Telegram&) { return false; }
};

//------------------------------------------------------------------------



#endif