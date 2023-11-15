#pragma once
/*
plasm2.h
moth
15 November 2023

The Moth Operating System
(c) Noah Wooten 2023, All Rights Reserved

This file is licensed under the MothOSL.
https://github.com/noahw2021/moth/blob/master/LICENSE

Changes:
11/15/23 : File created - nw
*/

#ifndef _MOTHTYPES
#define _MOTHTYPES

typedef unsigned __int64 WORD64, *PWORD64;
typedef unsigned __int32 WORD32, *PWORD32;
typedef unsigned __int16 WORD16, *PWORD16;
typedef unsigned __int8  CHAR, BYTE, *PCHAR, 
	*PBYTE, *PSTR;

#endif

/*
This file contains PLASM2-related structures
and instrinsics.
*/

typedef struct _PL2_CONTEXT {
	union {
		union {
			__int64 Registers64[32];
			struct {
				__int64 GPRs[16];
				__int64 Special[16];
			};
		};
		struct {
			__int64 GPRs[16];
			__int64 Ip;
			__int64 Sp;
			__int64 Flags;
			__int64 Security;

			struct {
				__int64 PageStart;
				__int64 PageEnd;
				__int64 ReturnAddressLocation;
				__int64 InterruptTable;
				__int64 VirtualStackPointer;
				__int64 CSMHandler;
				__int64 DevicePointer;
				__int64 StackPointerUpperBound;
				__int64 StackPointerLowerBound;
				__int64 PageMaxLocation;

				union {
					__int64 Reserved0;
					struct {
						__int64 InterruptReturn : 1;
						__int64 InInterrupt : 1;
						__int64 InTable : 1;

						__int64 Reserved : 61;
					};
				}PccelFlags;

				__int64 Reserved[1];
			}System;
		};
	};
}PL2_CONTEXT, *PPL2_CONTEXT;
