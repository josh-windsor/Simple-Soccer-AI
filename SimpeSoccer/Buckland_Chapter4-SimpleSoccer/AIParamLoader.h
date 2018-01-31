#ifndef AIPARAMLOADER
#define AIPARAMLOADER
#pragma warning(disable:4800)
//------------------------------------------------------------------------
//
//Name:  AIParamLoader.h
//
//Desc:  singleton class to handle the loading of default parameter
//       values from an initialization file: 'params.ini'
//
//Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <fstream>
#include <string>
#include <cassert>


#include "constants.h"
#include "Common/misc/iniFileLoaderBase.h"



class AIParamLoader : public iniFileLoaderBase
{

public:

	AIParamLoader(const char* file):iniFileLoaderBase(file)
  {    
	   
	
	ChancePlayerAttemptsPotShot = GetNextParameterDouble();
	ChanceOfUsingArriveTypeReceiveBehavior = GetNextParameterDouble();
	
	
	KeeperInBallRange           = GetNextParameterDouble();    
	PlayerInTargetRange         = GetNextParameterDouble(); 
	PlayerKickingDistance       = GetNextParameterDouble(); 
	PlayerKickFrequency         = GetNextParameterDouble();


	PlayerMass                  = GetNextParameterDouble(); 
	PlayerMaxForce              = GetNextParameterDouble();    
	PlayerMaxSpeedWithBall      = GetNextParameterDouble();   
	PlayerMaxSpeedWithoutBall   = GetNextParameterDouble();   
	PlayerMaxTurnRate           = GetNextParameterDouble();   
	PlayerScale                 = GetNextParameterDouble();      
	PlayerComfortZone           = GetNextParameterDouble();  
	PlayerKickingAccuracy       = GetNextParameterDouble();

	NumAttemptsToFindValidStrike = GetNextParameterInt();


	
	
	WithinRangeOfHome           = GetNextParameterDouble();    
	WithinRangeOfSupportSpot    = GetNextParameterDouble();    
	
	MinPassDist                 = GetNextParameterDouble();
	GoalkeeperMinPassDist       = GetNextParameterDouble();
	
	GoalKeeperTendingDistance   = GetNextParameterDouble();    
	GoalKeeperInterceptRange    = GetNextParameterDouble();
	BallWithinReceivingRange    = GetNextParameterDouble();
	

	BallWithinReceivingRangeSq = BallWithinReceivingRange * BallWithinReceivingRange;
	KeeperInBallRangeSq      = KeeperInBallRange * KeeperInBallRange;
	PlayerInTargetRangeSq    = PlayerInTargetRange * PlayerInTargetRange;   
	PlayerKickingDistanceSq  = PlayerKickingDistance * PlayerKickingDistance;
	PlayerComfortZoneSq      = PlayerComfortZone * PlayerComfortZone;
	GoalKeeperInterceptRangeSq     = GoalKeeperInterceptRange * GoalKeeperInterceptRange;
	WithinRangeOfSupportSpotSq = WithinRangeOfSupportSpot * WithinRangeOfSupportSpot;
  }
  
public:


  double ChancePlayerAttemptsPotShot; 
  double ChanceOfUsingArriveTypeReceiveBehavior;

  double KeeperInBallRange;
  double KeeperInBallRangeSq;

  double PlayerInTargetRange;
  double PlayerInTargetRangeSq;
  
  double PlayerMass;
  
  //max steering force
  double PlayerMaxForce; 
  double PlayerMaxSpeedWithBall;
  double PlayerMaxSpeedWithoutBall;
  double PlayerMaxTurnRate;
  double PlayerScale;
  double PlayerComfortZone;

  double PlayerKickingDistance;
  double PlayerKickingDistanceSq;

  double PlayerKickFrequency; 


  double  PlayerComfortZoneSq;

  //in the range zero to 1.0. adjusts the amount of noise added to a kick,
  //the lower the value the worse the players get
  double  PlayerKickingAccuracy;

  //the number of times the SoccerTeam::CanShoot method attempts to find
  //a valid shot
  int    NumAttemptsToFindValidStrike;

  //the distance away from the center of its home region a player
  //must be to be considered at home
  double WithinRangeOfHome;

  //how close a player must get to a sweet spot before he can change state
  double WithinRangeOfSupportSpot;
  double WithinRangeOfSupportSpotSq;
 
  
  //the minimum distance a receiving player must be from the passing player
  double   MinPassDist;
  double   GoalkeeperMinPassDist;

  //this is the distance the keeper puts between the back of the net
  //and the ball when using the interpose steering behavior
  double  GoalKeeperTendingDistance;

  //when the ball becomes within this distance of the goalkeeper he
  //changes state to intercept the ball
  double  GoalKeeperInterceptRange;
  double  GoalKeeperInterceptRangeSq;

  //how close the ball must be to a receiver before he starts chasing it
  double  BallWithinReceivingRange;
  double  BallWithinReceivingRangeSq;



};

#endif