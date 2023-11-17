#pragma once
/*
mot.h
moth
17 November 2023

The Moth Operating System
(c) Noah Wooten 2023, All Rights Reserved

This file is licensed under the MothOSL.
https://github.com/noahw2021/moth/blob/master/LICENSE

Changes:
11/17/23 : File created - nw
*/

#ifndef _MOTHTYPES
#define _MOTHTYPES

typedef unsigned __int64 WORD64, * PWORD64;
typedef unsigned __int32 WORD32, * PWORD32;
typedef unsigned __int16 WORD16, * PWORD16;
typedef unsigned __int8  CHAR, BYTE, * PCHAR,
* PBYTE, * PSTR, BOOLEAN, * PBOOLEAN;
typedef void* PVOID;
typedef wchar_t WCHAR, * PWSTR;

#define VOID void

#endif

#define MOTH_VERSION_MAJOR
#define MOTH_VERSION_MINOR
#define MOTH_VERSION_BUILD

#define MOTH_PLATFORM_AMD64CCEL 0x01
#define MOTH_PLATFORM_PLASMNATV 0x02

#define MOTH_PLATFORM MOTH_PLATFORM_AMD64CCEL
