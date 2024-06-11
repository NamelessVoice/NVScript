#ifndef BUILDCONFIG_H
#define BUILDCONFIG_H

#pragma once


#if GAME == 1
// T1
#define _DARKGAME 1
#define GAMENAME "Thief1"
#elif GAME == 3
// SS2
#define _DARKGAME 3
#define GAMENAME "Shock2"
#else
// T2
#define _DARKGAME 2
#define GAMENAME "Thief2"
#endif


#ifdef MODULE_NVSCRIPT
// NVScript version defines
#define SCRIPT_VERSION		1,3,1,0
#define SCRIPT_VERSTRING	"1.3.1"
#define SCRIPT_NAME			"NVScript Script Module"
#define SCRIPT_LONGNAME		"NVScript " GAMENAME " OSM"
#define SCRIPT_COPYRIGHT	"Copyright (C) 2024 Nameless Voice"
#elif defined(MODULE_NVDEBUG)
// NVDebug version defines
#define SCRIPT_VERSION		1,0,0,0
#define SCRIPT_VERSTRING	"1.0.0"
#define SCRIPT_NAME			"NVDebug Script Module"
#define SCRIPT_LONGNAME		"NVDebug " GAMENAME " OSM"
#define SCRIPT_COPYRIGHT	"Copyright (C) 2024 Nameless Voice"
#else
#ifdef RC_INVOKED
#error buildconfig.h not updated for current module.
#endif
#endif


#define _CRT_SECURE_NO_WARNINGS
// disable "POSIX name for this item is deprecated" warnings
#pragma warning(disable : 4996)

// enable access to M_PI etc.
#define _USE_MATH_DEFINES


#endif
