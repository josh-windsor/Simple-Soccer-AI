#ifndef AIPARAMTEAMLOADER
#define AIPARAMTEAMLOADER
#pragma warning(disable:4800)
//------------------------------------------------------------------------
//
//Name:  AIParamTeamLoader.h
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



class AIParamTeamLoader : public iniFileLoaderBase
{

public:

	AIParamTeamLoader(const char* file) :iniFileLoaderBase(file)
	{


		AdvancedFormations = (bool)GetNextParameterInt();
		StateTactics = GetNextParameterInt();
		Hemming = GetNextParameterInt();
		MarkDist = GetNextParameterDouble();

	}

public:


	bool AdvancedFormations;
	bool StateTactics;
	bool Hemming;
	double MarkDist;


};

#endif