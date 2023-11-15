#pragma once
/*
mintrnl.h
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
This file contains internal structures used by the
moth Operating System.
*/

// Required Inclusions
#include "plasm2.h"

// Predeclare Useful Structures
typedef struct _SYSTEM_THEAD* PSYSTEM_THREAD;
typedef struct _SYSTEM_DRIVER* PSYSTEM_DRIVER;
typedef struct _MEMORY_CHUNK* PMEMORY_CHUNK;
typedef struct _SYSTEM_CALL* PSYSTEM_CALL;
typedef struct _SYSTEM_TIME* PSYSTEM_TIME;
typedef struct _SYSTEM_HANDLE* PSYSTEM_HANDLE;
typedef struct _SCHEDULE_CONTEXT* PSCHEDULE_CONTEXT;
typedef struct _SCHEDULE_ENTITY* PSCHEDULE_ENTITY;
typedef struct _SCHEDULE_SYSTEM_EVENT* PSCHEDULE_SYSTEM_EVENT;
typedef struct _MOTH_EXEC_HEADER* PMOTH_EXEC_HEADER;
typedef struct _MOTH_EXEC_IMPORTTABLE* PMOTH_EXEC_IMPORTTABLE;
typedef struct _MOTH_EXEC_EXPORTTABLE* PMOTH_EXEC_EXPORTTABLE;
typedef struct _MOTH_EXEC_DEBUGTABLE* PMOTH_EXEC_DEBUGTABLE;
typedef struct _MOTH_EXEC_SECTION* PMOTH_EXEC_SECTION;
typedef struct _MOTH_EXEC_DIRECTORY* PMOTH_EXEC_DIRECTORY;
typedef struct _MOTH_EXEC_CERTIFICATE* PMOTH_EXEC_CERTIFICATE;
typedef struct _MOTH_EXEC_DEBUGENTRY* PMOTH_EXEC_DEBUGENTRY;
typedef struct _USER_PROCESS* PUSER_PROCESS;
typedef struct _USER_SESSION* PUSER_SESSION;
typedef struct _USER_THREAD* PUSER_THREAD;
typedef struct _USER_LIBRARY* PUSER_LIBRARY;
typedef struct _USER_HANDLE* PUSER_HANDLE;
typedef struct _USER_ISC_PROCESS* PUSER_ISC_PROCESS;
typedef struct _USER_ISC_SESSION* PUSER_ISC_SESSION;
typedef struct _USER_ISC_THREAD* PUSER_ISC_THREAD;
typedef struct _USER_ISC_LIBRARY* PUSER_ISC_LIBRARY;
typedef struct _SYSTEM_VIDEO_COMMAND* PSYSTEM_VIDEO_COMMAND;
typedef struct _SYSTEM_VIDEO_CONTEXT* PSYSTEM_VIDEO_CONTEXT;
typedef struct _SYSTEM_INPUT_COMMAND* PSYSTEM_INPUT_COMMAND;
typedef struct _SYSTEM_INPUT_CONTEXT* PSYSTEM_INPUT_CONTEXT;
typedef struct _SYSTEM_DEVICE_EVENT* PSYSTEM_DEVICE_EVENT;
typedef struct _SYSTEM_DEVICE* PSYSTEM_DEVICE;
typedef struct _SYSTEM_DEVICE_CONTROL* PSYSTEM_DEVICE_CONTROL;
typedef struct _SYSTEM_DEVICE_MANAGER* PSYSTEM_DEVICE_MANAGER;
typedef struct _SYSTEM_MEMORY_HEAP* PSYSTEM_MEMORY_HEAP;
typedef struct _SYSTEM_MEMORY_ALLOCATION* PSYSTEM_MEMORY_ALLOCATION;
typedef struct _SYSTEM_MEMORY_GARBAGELIST_ENTRY*
	PSYSTEM_MEMORY_GARBAGELIST_ENTRY;
typedef struct _SYSTEM_MEMORY_GARBAGE_MANAGER*
	PSYSTEM_MEMORY_GARBAGE_MANAGER;

// Driver Structures

typedef struct _KDRIVER_IMPORT {
	char ModuleName[16];
	char ImportName[48];

	WORD64 LoaderCallValue;
	WORD64 ReferenceCount;
	WORD64 IntUse_ResolvedAddr;
}KDRIVER_IMPORT, *PKDRIVER_IMPORT;

typedef struct _KDRIVER_EXPORT {
	char ExportName[48];
	WORD64 RelativeBase;

	WORD64 ExportAddress;
}KDRIVER_EXPORT, *PKDRIVER_EXPORT;	

typedef struct _KDRIVER_EXEC_HDR {
	WORD16 Magic;
	CHAR ModuleName[16];

	WORD64 ImportDirectoryVirtual;
	WORD64 ImportDirectorySize;
	WORD64 ExportDirectoryVirtual;
	WORD64 ExportDirectorySize;

	WORD64 MinimumRequiredHeap;
	WORD64 EntryPoint;
	WORD64 MinimumKernelVersion;
	WORD64 CompileTime;

	WORD64 Extension;
	WORD64 Reserved[3];
}KDRIVER_EXEC_HDR, *PKDRIVER_EXEC_HDR;

// Internal Kernel Structures

typedef struct _SYSTEM_THREAD { // System CPU Structure
	SYSTEM_TIME LastTick;

	PSCHEDULE_CONTEXT ThisScheduler;
	
	PUSER_THREAD* UserThreads;
	WORD32 UserThreadCount;
}SYSTEM_THREAD, *PSYSTEM_THREAD;

typedef struct _SYSTEM_DRIVER { // Kernel Driver (KM Process) Structure
	PKDRIVER_EXPORT ExportTable;
	PKDRIVER_IMPORT ImportTable;
	PKDRIVER_EXEC_HDR ExecHeader;

	PSYSTEM_MEMORY_HEAP DriverHeap;
	PSYSTEM_DEVICE LinkedDevice;

	PSYSTEM_HANDLE MySystemHandle;
	PUSER_HANDLE* MyDestinationUserHandles; // list of handles to the driver
	WORD32 MyDestinationUserHandleCount;

	PSYSTEM_DEVICE_CONTROL ControlEvents;
	WORD32 ControlEventCount;
}SYSTEM_DRIVER, *PSYSTEM_DRIVER;

typedef struct _SYSTEM_INFO { // System Information Structure
	PSYSTEM_THREAD PrimaryCpu;
	PSYSTEM_THREAD* Cpus;
	WORD16 CpuCount;

	PSYSTEM_DRIVER* Drivers;
	DWORD32 DriverCount;

	PMEMORY_CHUNK* MemoryMaps;
	PWORD64 RefMemoryMapSizes;
	WORD64 MemoryMapCount;

	PSYSTEM_CALL* PendingSystemCalls;
	WORD64 PendingSysCallCount;

	PSYSTEM_TIME CurrentSystemTime;
	PSCHEDULE_CONTEXT PrimarySystemScheduler;
	PSCHEDULE_CONTEXT* SchedulerByCpu;
	WORD32 IndividualSchedulerCount;
	PUSER_SESSION UserSessions;
	WORD16 UserSessionCount;

	PSYSTEM_INPUT_CONTEXT InputCtx;
	PSYSTEM_VIDEO_CONTEXT VideoCtx;
	PSYSTEM_DEVICE_MANAGER Devices;
	PSYSTEM_MEMORY_HEAP KernelHeaps;
}SYSTEM_INFO, *PSYSTEM_INFO;

typedef struct _MEMORY_CHUNK { // Memory Chunk Structure
	WORD64 BasePhysicalAddress;
	WORD64 ChunkSize;

	union {
		WORD64 FlagsRaw;
		struct {
			WORD64 InUse : 1;

			word64 Reserved : 63;
		};
	}Flags;

	WORD64 Reserved;
}MEMORY_CHUNK, *PMEMORY_CHUNK;

typedef struct _SYSTEM_CALL { // System Call Information
	WORD64 SystemCallId;
	WORD64 Arguments[4];
	WORD64 ReturnPtr;

	PSYSTEM_HANDLE Initiator; // likely a user-application
	WORD64 ReturnIp;
}SYSTEM_CALL, *PSYSTEM_CALL;

typedef struct _SYSTEM_TIME { // System Time
	WORD64 SecondsSince2000;
	WORD64 MsSince2000;
	WORD64 NsSince2023;
	WORD64 NsSinceBoot;
}SYSTEM_TIME, *PSYSTEM_TIME; 

#define SYSHND_USERPROCESS 0x00
#define SYSHND_USERSESSION 0x01
#define SYSHND_USERLIBRARY 0x02
#define SYSHND_USERTHREAD  0x03
#define SYSHND_USERMOTHAPI 0x04
#define SYSHND_KERNDRIVER  0x05
#define SYSHND_KERNFILE    0x06
#define SYSHND_KERNHEAP    0x07
#define SYSHND_KERNGCLIST  0x08

#define SYSHND_OWNER_SYSTEM 0x13220000ABCD037F

typedef struct _SYSTEM_HANDLE { // Kernel-mode handle
	BYTE HandleType;
	WORD64 RelevantInformation;
	WORD64 MothHandleType; // api level handle type
	union {
		WORD64 Value;
		PSYSTEM_HANDLE Handle;
	}HandleOwner;

	union {
		PUSER_PROCESS Process;
		PUSER_SESSION Sesison;
		PUSER_LIBRARY Library;
		PUSER_THREAD Thread;
		VOID* MothApi;
		PSYSTEM_DRIVER KDriver;
		PSYSTEM_MEMORY_HEAP KHeap;
		PSYSTEM_MEMORY_GARBAGE_MANAGER GcList;
	}Structure;
}SYSTEM_HANDLE, *PSYSTEM_HANDLE;

// Internal Kernel Scheduler Structures

typedef struct _SCHEDULE_CONTEXT { // Scheduler Context
	PSYSTEM_THREAD LocalCpu;

	PSCHEDULE_ENTITY Entities;
	WORD32 EntityCount;

	PSCHEDULE_ENTITY* HighestPriorities;
	WORD32 HighPriorityCount;
}SCHEDULE_CONTEXT, *PSCHEDULE_CONTEXT;

typedef struct _SCHEDULE_ENTITY { // Scheduler Entity
	PL2_CONTEXT EntityContext;
	DWORD32 EntityPriority;
	SYSTEM_TIME LastExecution;
	BYTE IsDisabled;
	BYTE IsSleeping;
	BYTE IsSpecialEvent;

	SYSTEM_TIME AwakeTime;
	PSCHEDULE_SYSTEM_EVENT SpecialEvent;
}SCHEDULE_ENTITY, *PSCHEDULE_ENTITY;

typedef struct _SCHEDULE_SYSTEM_EVENT { // Scheduler Kernel Event
	WORD64 Reserved;
}SCHEDULE_SYSTEM_EVENT; 

// External Executable File Header Structures

#define MOTHDIR_IMPORTS  0x00
#define MOTHDIR_EXPORTS  0x01
#define MOTHDIR_DEBUG    0x02
#define MOTHDIR_CODESIGN 0x03
#define MOTHDIR_SECTIONS 0x04
#define MOTHDIR_XTENSION 0x05

typedef struct _MOTH_EXEC_DIRECTORY { // Executable Directory Structure
	WORD64 VirtualAddress;
	WORD64 Size;
}MOTH_EXEC_DIRECTORY, *PMOTH_EXEC_DIRECTORY;

typedef struct _MOTH_EXEC_HEADER { // Executable Header
	// Critical Information
	WORD16 Magic;
	MOTH_EXEC_DIRECTORY DataDirectories[16];
	
	// Executible Loader Information
	WORD64 BaseAddress;
	WORD64 SizeOfCode;
	WORD64 EntryPoint;

	// Executible Attributes
	BYTE ExecutibleType;
	WORD64 CompileTime;
	CHAR CompilerMark[32];
	WORD64 MinimumKernelVersion;
	WORD64 MinimumUserVersion;
	WORD64 Checksum;
	WORD64 MinimumRequiredAllocation;
	WORD64 MaximumLikelyAllocation;
}MOTH_EXEC_HEADER, *PMOTH_EXEC_HEADER;

typedef struct _MOTH_EXEC_IMPORTTABLE { // Executable Import Table
	CHAR ModuleName[32];
	CHAR ImportName[48];

	WORD64 LoaderCallValue; // Random number indicating an assignment is required.
	WORD64 ReferenceCount;
	
	struct {
		union {
			WORD64 Values[4];
			struct {
				WORD64 ResolvedValue;
				WORD64 ReferencesFound;
				WORD64 ModuleBaseAddress;
				WORD64 ModuleHandle;
			};
		};
	}RuntimeUseOnly;
}MOTH_EXEC_IMPORTTABLE, *PMOTH_EXEC_IMPORTTABLE;

typedef struct _MOTH_EXEC_EXPORTTABLE { // Executable Export Table
	CHAR ExportName[48];

	WORD64 AbsoluteAddress;
	WORD64 RelativeToBase;

	struct {
		union {
			WORD64 Values[2];
			struct {
				WORD64 ResolvedValue;
				WORD64 ModuleBaseAddress;
			};
		};
	}RuntimeUseOnly;
}MOTH_EXEC_EXPORTTABLE, *PMOTH_EXEC_EXPORTTABLE;

#define MOTH_EXECDT_ICFLAG_FUNCTIONS       0x01
#define MOTH_EXECDT_ICFLAG_SOURCECODE      0x02
#define MOTH_EXECDT_ICFLAG_BREAKPOINTS     0x04
#define MOTH_EXECDT_ICFLAG_LINEINFORMATION 0x08
#define MOTH_EXECDT_ICFLAG_COMMENTS        0x10

typedef struct _MOTH_EXEC_DEBUGTABLE { // Executable Debug Table
	BYTE IsOnlyStub; // if non-zero, only this header exists and no useful data.
	BYTE InformationContained; 

	MOTH_EXEC_DIRECTORY EntriesDirectory;
	DWORD32 EntryCount;
}MOTH_EXEC_DEBUGTABLE, *PMOTH_EXEC_DEBUGTABLE;

typedef struct _MOTH_EXEC_SECTION { // Executable Sections
	struct {
		WORD64 Previous;
		WORD64 Me;
		WORD64 Next;
	}LinkedData;

	CHAR SectionName[16];
	WORD64 VirtualAddress;
	WORD64 Size;
	WORD64 BaseAddress;
	BYTE Permissions;
}MOTH_EXEC_SECTION, *PMOTH_EXEC_SECTION;

typedef struct _MOTH_EXEC_CERTIFICATE { // Codesigning Certificate
	WORD64 CertificateSerials[4];
	WORD64 CertificateChecksum;

	CHAR NameProgram[32];
	CHAR NameAuthor[32];
	WORD64 SignatureTime;

	WORD64 SignatureProofVirtualAddress;
	WORD64 VirtualSize;
}MOTH_EXEC_CERTIFICATE, *PMOTH_EXEC_CERTIFICATE;

typedef struct _MOTH_EXEC_DEBUGENTRY { // Debug Entries
	BYTE EntryType;
	WORD64 RelativeBase; // = Header->BaseAddress

	WORD64 LinkedRelativeIpLow;
	WORD64 LinkedRelativeIpHigh;

	MOTH_EXEC_DIRECTORY DataDirectory;
}MOTH_EXEC_DEBUGENTRY, *PMOTH_EXEC_DEBUGENTRY;

// Internal User Process Structures

typedef struct _USER_PROCESS { // User-mode Process Information
	WORD64 Reserved;
}USER_PROCESS, *PUSER_PROCESS;

typedef struct _USER_SESSION { // User-mode Session Information
	WORD64 Reserved;
}USER_SESSION, *PUSER_SESSION;

typedef struct _USER_THREAD { // User-mode Thread Information
	WORD64 Reserved;
}USER_THREAD, *PUSER_THREAD;

typedef struct _USER_LIBRARY { // User-mode Handle Information
	WORD64 Reserved;
}USER_LIBRARY, *PUSER_LIBRARY;

typedef struct _USER_ISC_PROCESS { // Inter-State Communication : Process
	WORD64 Reserved;
}USER_ISC_PROCESS, *PUSER_ISC_PROCESS;

typedef struct _USER_ISC_SESSION { // Inter-State Communication : Session
	WORD64 Reserved;
}USER_ISC_SESSION, *PUSER_ISC_SESSION;

typedef struct _USER_ISC_THREAD { // Inter-State Communication : Thread
	WORD64 Reserved;
}USER_ISC_THREAD, *PUSER_ISC_THREAD;

typedef struct _USER_ISC_LIBRARY { // Inter-State Communication : Library
	WORD64 Reserved;
}USER_ISC_LIBRARY, *PUSER_ISC_LIBRARY;

typedef struct _USER_HANDLE { // User-mode handle

}USER_HANDLE, *PUSER_HANDLE;

// Kernel API Information

typedef struct _SYSTEM_VIDEO_COMMAND { // Kernel Video Manager : Command
	WORD64 Reserved;
}SYSTEM_VIDEO_COMMAND, *PSYSTEM_VIDEO_COMMAND;

typedef struct _SYSTEM_VIDEO_CONTEXT { // Kernel Video Manager : Context
	WORD64 Reserved;
}SYSTEM_VIDEO_CONTEXT, *PSYSTEM_VIDEO_CONTEXT;

typedef struct _SYSTEM_INPUT_COMMAND { // Kernel Input Manager : Command
	WORD64 Reserved;
}SYSTEM_INPUT_COMMAND, *PSYSTEM_INPUT_COMMAND;

typedef struct _SYSTEM_INPUT_CONTEXT { // Kernel Input Manager : Context
	WORD64 Reserved;
}SYSTEM_INPUT_CONTEXT, *PSYSTEM_INPUT_CONTEXT;

typedef struct _SYSTEM_DEVICE_EVENT { // Kernel Device Manager : Device Event
	WORD64 Reserved;
}SYSTEM_DEVICE_EVENT, *PSYSTEM_DEVICE_EVENT;

typedef struct _SYSTEM_DEVICE { // Kernel Device Manager : Device
	WORD64 Reserved;
}SYSTEM_DEVICE, *PSYSTEM_DEVICE;

typedef struct _SYSTEM_DEVICE_MANAGER { // Kernel Device Manager : Device Manager
	WORD64 Reserved;
}SYSTEM_DEVICE_MANAGER, *PSYSTEM_DEVICE_MANAGER;

typedef struct _SYSTEM_DEVICE_CONTROL { // Kernel-User Mode ISC
	BYTE Originator; // 0 = Driver, 1 = Program
	
	PSYSTEM_HANDLE OriginSystemHandle;
	PUSER_HANDLE TargetLocalHandle;

	WORD64 InformationPacket[8];
	WORD64 Command;
}

// Memory Information

typedef struct _SYSTEM_MEMORY_HEAP { // Memory Manager : System Heap
	WORD64 Reserved;
}SYSTEM_MEMORY_HEAP, *PSYSTEM_MEMORY_HEAP;

typedef struct _SYSTEM_MEMORY_ALLOCATION { // Memory Manager : System Heap Allocation
	WORD64 Reserved;
}SYSTEM_MEMORY_ALLOCATION, *PSYSTEM_MEMORY_ALLOCATION;

typedef struct _SYSTEM_MEMORY_GARBAGELIST_ENTRY { // Memory Manager : Heap Garbage List Entry
	WORD64 Reserved;
}SYSTEM_MEMORY_GARBAGELIST_ENTRY, 
*PSYSTEM_MEMORY_GARBAGELIST_ENTRY;

typedef struct _SYSTEM_MEMORY_GARBAGE_MANAGER { // Memory Manager : Heap Garbage Manager
	WORD64 Reserved;
}SYSTEM_MEMORY_GARBAGE_MANAGER,
*PSYSTEM_MEMORY_GARBAGE_MANAGER;