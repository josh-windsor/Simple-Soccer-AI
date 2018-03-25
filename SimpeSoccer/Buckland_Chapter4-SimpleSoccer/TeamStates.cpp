#include "TeamStates.h"
#include "SoccerTeam.h"
#include "PlayerBase.h"
#include "Common/Messaging/MessageDispatcher.h"
#include "SoccerMessages.h"
#include "constants.h"
#include "SoccerPitch.h"
#include "Goal.h"

//uncomment to send state info to debug window
//#define DEBUG_TEAM_STATES
#include "Common/Debug/DebugConsole.h"




void ChangePlayerHomeRegions(SoccerTeam* team, const int NewRegions[TeamSize])
{
  for (int plyr=0; plyr<TeamSize; ++plyr)
  {
	team->SetPlayerHomeRegion(plyr, NewRegions[plyr]);
  }
}



//************************************************************************ ATTACKING

Attacking* Attacking::Instance()
{
  static Attacking instance;

  return &instance;
}


void Attacking::Enter(SoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " entering Attacking state" << "";
#endif

  //these define the home regions for this state of each of the players
  const int BlueRegions[TeamSize] = {1,12,14,6,4};
  const int RedRegions[TeamSize] = {16,3,5,9,13};

  //set up the player's home regions
  if (team->Color() == SoccerTeam::blue)
  {
	ChangePlayerHomeRegions(team, BlueRegions);
  }
  else
  {
	ChangePlayerHomeRegions(team, RedRegions);
  }

  //if a player is in either the Wait or ReturnToHomeRegion states, its
  //steering target must be updated to that of its new home region to enable
  //it to move into the correct position.
  team->UpdateTargetsOfWaitingPlayers();
}


void Attacking::Execute(SoccerTeam* team)
{
	if (team->m_pTeamParamFile->AdvancedFormations)
	{

		//check if the team is losing
		if (team->HomeGoal()->NumGoalsScored() < team->OpponentsGoal()->NumGoalsScored())
		{
			team->Pitch()->SetDifficulty(team->Color() == team->red, 2);
			team->GetFSM()->ChangeState(Losing::Instance()); return;
		}
		//check if the team is winning
		else if (team->HomeGoal()->NumGoalsScored() > team->OpponentsGoal()->NumGoalsScored())
		{
			team->Pitch()->SetDifficulty(team->Color() == team->red, 0);
			team->GetFSM()->ChangeState(Winning::Instance()); return;
		}
	}
  //if this team is no longer in control change states
  if (!team->InControl())
  {
	team->GetFSM()->ChangeState(Defending::Instance()); return;
  }

  //calculate the best position for any supporting attacker to move to
  team->DetermineBestSupportingPosition();
}

void Attacking::Exit(SoccerTeam* team)
{
  //there is no supporting player for defense
  team->SetSupportingPlayer(NULL);
}



//************************************************************************ DEFENDING

Defending* Defending::Instance()
{
  static Defending instance;

  return &instance;
}

void Defending::Enter(SoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " entering Defending state" << "";
#endif

  //these define the home regions for this state of each of the players
  const int BlueRegions[TeamSize] = {1,6,8,3,5};
  const int RedRegions[TeamSize] = {16,9,11,12,14};

  //set up the player's home regions
  if (team->Color() == SoccerTeam::blue)
  {
	ChangePlayerHomeRegions(team, BlueRegions);
  }
  else
  {
	ChangePlayerHomeRegions(team, RedRegions);
  }
  
  //if a player is in either the Wait or ReturnToHomeRegion states, its
  //steering target must be updated to that of its new home region
  team->UpdateTargetsOfWaitingPlayers();
}

void Defending::Execute(SoccerTeam* team)
{
	if (team->m_pTeamParamFile->AdvancedFormations)
	{
		if (team->HomeGoal()->NumGoalsScored() < team->OpponentsGoal()->NumGoalsScored())
		{
			team->Pitch()->SetDifficulty(team->Color() == team->red, 2);
			team->GetFSM()->ChangeState(Losing::Instance()); return;
		}
		else if (team->HomeGoal()->NumGoalsScored() > team->OpponentsGoal()->NumGoalsScored())
		{
			team->Pitch()->SetDifficulty(team->Color() == team->red, 0);
			team->GetFSM()->ChangeState(Winning::Instance()); return;
		}
	}

  //if in control change states
  if (team->InControl())
  {
	team->GetFSM()->ChangeState(Attacking::Instance()); return;
  }
}


void Defending::Exit(SoccerTeam* team){}


//************************************************************************ KICKOFF
PrepareForKickOff* PrepareForKickOff::Instance()
{
  static PrepareForKickOff instance;

  return &instance;
}

void PrepareForKickOff::Enter(SoccerTeam* team)
{
  //reset key player pointers
  team->SetControllingPlayer(NULL);
  team->SetSupportingPlayer(NULL);
  team->SetReceiver(NULL);
  team->SetPlayerClosestToBall(NULL);

  //send Msg_GoHome to each player.
  team->ReturnAllFieldPlayersToHome();
}

void PrepareForKickOff::Execute(SoccerTeam* team)
{
  //if both teams in position, start the game
  if (team->AllPlayersAtHome() && team->Opponents()->AllPlayersAtHome())
  {
	team->GetFSM()->ChangeState(Defending::Instance());
  }
}

void PrepareForKickOff::Exit(SoccerTeam* team)
{
  team->Pitch()->SetGameOn();
}

//************************************************************************ KICKOFF

Winning* Winning::Instance()
{
	static Winning instance;

	return &instance;
}

void Winning::Enter(SoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
	debug_con << team->Name() << " entering Winning state" << "";
#endif

	//these define the home regions for this state of each of the players
	const int BlueRegions[TeamSize] = { 1,12,14,3,5 };
	const int RedRegions[TeamSize] = { 16,3,5,12,14 };


	//set up the player's home regions
	if (team->Color() == SoccerTeam::blue)
	{
		ChangePlayerHomeRegions(team, BlueRegions);
	}
	else
	{
		ChangePlayerHomeRegions(team, RedRegions);
	}

	//if a player is in either the Wait or ReturnToHomeRegion states, its
	//steering target must be updated to that of its new home region
	team->UpdateTargetsOfWaitingPlayers();
}

void Winning::Execute(SoccerTeam* team)
{
	if (team->m_pTeamParamFile->AdvancedFormations)
	{
		//if in control change states
		if (team->HomeGoal()->NumGoalsScored() < team->OpponentsGoal()->NumGoalsScored())
		{
			team->Pitch()->SetDifficulty(team->Color() == team->red, 2);
			team->GetFSM()->ChangeState(Losing::Instance()); return;
		}
		else if (team->HomeGoal()->NumGoalsScored() == team->OpponentsGoal()->NumGoalsScored())
		{
			team->Pitch()->SetDifficulty(team->Color() == team->red, 1);
			//if in control change states
			if (team->InControl())
			{
				team->GetFSM()->ChangeState(Attacking::Instance()); return;
			}
			else
			{
				team->GetFSM()->ChangeState(Defending::Instance()); return;
			}
		}
	}
	else
	{
		if (team->InControl())
		{
			team->GetFSM()->ChangeState(Attacking::Instance()); return;
		}
		else
		{
			team->GetFSM()->ChangeState(Defending::Instance()); return;
		}

	}
}


void Winning::Exit(SoccerTeam* team) {}

//************************************************************************ KICKOFF

Losing* Losing::Instance()
{
	static Losing instance;

	return &instance;
}

void Losing::Enter(SoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
	debug_con << team->Name() << " entering Winning state" << "";
#endif

	//these define the home regions for this state of each of the players
	const int BlueRegions[TeamSize] = { 1,3,5,0,2 };
	const int RedRegions[TeamSize] = { 16,12,14,15,17 };

	//set up the player's home regions
	if (team->Color() == SoccerTeam::blue)
	{
		ChangePlayerHomeRegions(team, BlueRegions);
	}
	else
	{
		ChangePlayerHomeRegions(team, RedRegions);
	}

	//if a player is in either the Wait or ReturnToHomeRegion states, its
	//steering target must be updated to that of its new home region
	team->UpdateTargetsOfWaitingPlayers();
}

void Losing::Execute(SoccerTeam* team)
{
	if (team->m_pTeamParamFile->AdvancedFormations)
	{
		//if in control change states
		if (team->HomeGoal()->NumGoalsScored() > team->OpponentsGoal()->NumGoalsScored())
		{
			team->Pitch()->SetDifficulty(team->Color() == team->red, 0);
			team->GetFSM()->ChangeState(Winning::Instance()); return;
		}
		else if (team->HomeGoal()->NumGoalsScored() == team->OpponentsGoal()->NumGoalsScored())
		{
			team->Pitch()->SetDifficulty(team->Color() == team->red, 1);

			//if in control change states
			if (team->InControl())
			{
				team->GetFSM()->ChangeState(Attacking::Instance()); return;
			}
			else
			{
				team->GetFSM()->ChangeState(Defending::Instance()); return;
			}
		}
	}
	else
	{
		if (team->InControl())
		{
			team->GetFSM()->ChangeState(Attacking::Instance()); return;
		}
		else
		{
			team->GetFSM()->ChangeState(Defending::Instance()); return;
		}
	}
}


void Losing::Exit(SoccerTeam* team) {}