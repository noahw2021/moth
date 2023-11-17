#pragma once
/*
ccel.h
moth
17 November 2023

The Moth Operating System
(c) Noah Wooten 2023, All Rights Reserved

This file is licensed under the MothOSL.
https://github.com/noahw2021/moth/blob/master/LICENSE

Changes:
11/17/23 : File created - nw
*/

#if (MOTH_PLATFORM == MOTH_PLATFORM_AMD64CCEL)
#define CCEL_IMPORTS_MODE
#include "../../Pccel/Pccel/exports.h"
#else
#error "Invalid Target: Do not include AMD64CCEL Files!"
#endif