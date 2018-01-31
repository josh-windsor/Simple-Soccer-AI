#ifndef GLOBALPARAMLOADER
#define GLOBALPARAMLOADER
#pragma warning(disable:4800)



#include <fstream>
#include <string>
#include <cassert>


#include "constants.h"
#include "Common/misc/iniFileLoaderBase.h"


#define Prm (*GlobalParamLoader::Instance())

class GlobalParamLoader : public iniFileLoaderBase
{

private:

	GlobalParamLoader() :iniFileLoaderBase("Params.ini")
	{

		GoalWidth = GetNextParameterDouble();

		NumSupportSpotsX = GetNextParameterInt();
		NumSupportSpotsY = GetNextParameterInt();

		Spot_PassSafeScore = GetNextParameterDouble();
		Spot_CanScoreFromPositionScore = GetNextParameterDouble();
		Spot_DistFromControllingPlayerScore = GetNextParameterDouble();
		Spot_ClosenessToSupportingPlayerScore = GetNextParameterDouble();
		Spot_AheadOfAttackerScore = GetNextParameterDouble();

		SupportSpotUpdateFreq = GetNextParameterDouble();



		BallSize = GetNextParameterDouble();
		BallMass = GetNextParameterDouble();
		Friction = GetNextParameterDouble();



		MaxDribbleForce = GetNextParameterDouble();
		MaxShootingForce = GetNextParameterDouble();
		MaxPassingForce = GetNextParameterDouble();


		bStates = GetNextParameterBool();
		bIDs = GetNextParameterBool();
		bSupportSpots = GetNextParameterBool();
		bRegions = GetNextParameterBool();
		bShowControllingTeam = GetNextParameterBool();
		bViewTargets = GetNextParameterBool();
		bHighlightIfThreatened = GetNextParameterBool();

		FrameRate = GetNextParameterInt();

		SeparationCoefficient = GetNextParameterDouble();
		ViewDistance = GetNextParameterDouble();
		bNonPenetrationConstraint = GetNextParameterBool();


	}

public:

	static GlobalParamLoader* Instance();

	double GoalWidth;

	int   NumSupportSpotsX;
	int   NumSupportSpotsY;

	//these values tweak the various rules used to calculate the support spots
	double Spot_PassSafeScore;
	double Spot_CanScoreFromPositionScore;
	double Spot_DistFromControllingPlayerScore;
	double Spot_ClosenessToSupportingPlayerScore;
	double Spot_AheadOfAttackerScore;

	double SupportSpotUpdateFreq;


	double BallSize;
	double BallMass;
	double Friction;

	double  MaxDribbleForce;
	double  MaxShootingForce;
	double  MaxPassingForce;



	//these values control what debug info you can see
	bool  bStates;
	bool  bIDs;
	bool  bSupportSpots;
	bool  bRegions;
	bool  bShowControllingTeam;
	bool  bViewTargets;
	bool  bHighlightIfThreatened;

	int FrameRate;


	double SeparationCoefficient;

	//how close a neighbour must be before an agent perceives it
	double ViewDistance;

	//zero this to turn the constraint off
	bool bNonPenetrationConstraint;

};

#endif