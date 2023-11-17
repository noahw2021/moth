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
11/16/23 : Continue work - nw
*/

#include "mot.h"

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
typedef struct _USER_EXCEPTION_HANDLER_CALL*
	PUSER_EXCEPTION_HANDLER_CALL;

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
	WORD32 DriverCount;

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

typedef struct _STRING_PTR {
	PWSTR Buffer;
	WORD32 StringLengthChars;
	WORD32 MaxBufferLength;
}STRING_PTR;

typedef struct _STRING_PREALLOC {
	WCHAR Buffer[512];
	WORD32 StringLengthChars;
}STRING_PREALLOC;

typedef struct _MEMORY_CHUNK { // Memory Chunk Structure
	WORD64 BasePhysicalAddress;
	WORD64 ChunkSize;

	union {
		WORD64 FlagsRaw;
		struct {
			WORD64 InUse : 1;

			WORD64 Reserved : 63;
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
	STRING_PREALLOC HandleName;
	union {
		WORD64 Value;
		PSYSTEM_HANDLE Handle;
	}HandleOwner;

	union {
		PUSER_PROCESS Process;
		PUSER_SESSION Sesison;
		PUSER_LIBRARY Library;
		PUSER_THREAD Thread;
		void* MothApi;
		PSYSTEM_DRIVER KDriver;
		PSYSTEM_MEMORY_HEAP KHeap;
		PSYSTEM_MEMORY_GARBAGE_MANAGER GcList;
	}Structure;
}SYSTEM_HANDLE, *PSYSTEM_HANDLE;

// Internal Kernel Scheduler Structures

typedef struct _SCHEDULE_CONTEXT { // Scheduler Context
	PSYSTEM_THREAD LocalCpu;
	PSYSTEM_HANDLE SystemHandle;

	PSCHEDULE_ENTITY Entities;
	WORD32 EntityCount;

	PSCHEDULE_ENTITY* HighestPriorities;
	WORD32 HighPriorityCount;
}SCHEDULE_CONTEXT, *PSCHEDULE_CONTEXT;

typedef struct _SCHEDULE_ENTITY { // Scheduler Entity
	PL2_CONTEXT EntityContext;
	PSYSTEM_HANDLE SystemHandle;
	WORD32 EntityPriority;
	SYSTEM_TIME LastExecution;
	BYTE IsDisabled;
	BYTE IsSleeping;
	BYTE IsSpecialEvent;

	SYSTEM_TIME AwakeTime;
	PSCHEDULE_SYSTEM_EVENT SpecialEvent;
	PSYSTEM_HANDLE UserThread;
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
	WORD32 EntryCount;
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

typedef struct _USER_EXCEPTION_HANDLER {
	WORD64 HandlerOrder;
	PSYSTEM_HANDLE SystemHandle;

	PUSER_PROCESS ValidProcess;
	PUSER_THREAD ValidThread;
	PUSER_LIBRARY ValidLibrary;

	WORD64 FunctionAddress;
}USER_EXCEPTION_HANDLER, *PUSER_EXCEPTION_HANDLER;

typedef struct _USER_EXCEPTION_HANDLER_CALL {
	PL2_CONTEXT Context;
	WORD64 HandlerSkips;

	WORD64 ExceptionCode;
	WORD64 ExceptionArgument[4];
	WORD64 TranslatedErrorMessage;
	WORD64 ExceptionCallReturnAddress;
}USER_EXCEPTION_HANDLER_CALL,
*PUSER_EXCEPTION_HANDLER_CALL;

typedef struct _USER_PROCESS { // User-mode Process Information
	WORD64 ProcessId;
	PUSER_SESSION SessionOwner;
	CHAR ProcessName[64];
	CHAR CommandLine[512];
	PSYSTEM_HANDLE SystemHandle;
	PUSER_PROCESS ParentProcess;
	PUSER_PROCESS* ChildProcesses;
	WORD32 ChildProcessCount;
	WORD32 ProcessReturnCode;
	
	PUSER_HANDLE HandlesCreated;
	WORD32 HandleCount;
	PUSER_HANDLE* HandlesToMe;
	WORD32 HandlesToMeCount;

	union {
		WORD64 RawFlags;
		struct {
			WORD64 IsProcessSuspended : 1;
			WORD64 IsProcessBeingDebugged : 1;
			WORD64 IsProcessContainSymbols : 1;
			WORD64 IsProcessSigned : 1;
		};
	}Flags;

	PUSER_THREAD MainThread;
	PUSER_THREAD Threads;
	WORD32 ThreadCount;
	PUSER_LIBRARY Libraries;
	WORD32 LibaryCount;

	PMOTH_EXEC_HEADER ExecHeader;
	PMOTH_EXEC_IMPORTTABLE ImportTable;
	PMOTH_EXEC_EXPORTTABLE ExportTable;
	PMOTH_EXEC_DEBUGTABLE DebugTable;
	PMOTH_EXEC_CERTIFICATE Certificate;
	PMOTH_EXEC_SECTION SectionList;
	PMOTH_EXEC_SECTION* SectionsResolved;
	WORD32 SectionCount;

	PSYSTEM_MEMORY_HEAP PrimaryProcessHeap;
	PSYSTEM_MEMORY_ALLOCATION ProcessBase;
	PSYSTEM_MEMORY_HEAP* ProcessHeaps;
	WORD32 ProcessHeapCount;

	PUSER_ISC_PROCESS OriginatedProcessISC;
	PUSER_ISC_LIBRARY HostedLibraryISC;
	PUSER_ISC_THREAD  HostedThreadISC;
	WORD32 OriginatedProcessCount;
	WORD32 HostedLibraryCount;
	WORD32 HostedThreadCount;

	WORD32 ProcessExceptionHandlers;
	WORD64 LastErrorCode;
	PUSER_EXCEPTION_HANDLER ProcessExceptionHandlers;
	PUSER_EXCEPTION_HANDLER** ThreadExceptionHandlersMatrix;
	WORD32* ThreadExceptionHandlersMatrixPerThreadHandlerCount;
	PUSER_EXCEPTION_HANDLER* ThreadExceptionHandlersSerialized;
	WORD32 ThreadExceptionHandlersSerializedTotalCount;
}USER_PROCESS, *PUSER_PROCESS;

typedef struct _USER_SESSION { // User-mode Session Information
	WORD64 SessionId;
	STRING_PTR SessionName;
	PSYSTEM_HANDLE SystemHandle;

	PUSER_PROCESS Processes;
	WORD32 ProcessCount;
	PUSER_THREAD* Threads;
	WORD32 ThreadCount;
	PUSER_HANDLE* Handles;
	WORD32 HandleCount;

	PSYSTEM_MEMORY_HEAP* SessionHeaps;
	WORD32 SessionHeapCount;
	WORD64 EstimatedTotalUsage;
}USER_SESSION, *PUSER_SESSION;

typedef struct _USER_THREAD { // User-mode Thread Information
	WORD64 ThreadId;
	STRING_PTR ThreadName;
	PSYSTEM_HANDLE SystemHandle;
	PSCHEDULE_ENTITY ScheduleEntity;
	PUSER_PROCESS ProcessOwner;
	WORD32 ThreadState;
	WORD32 ThreadReturnCode;

	PUSER_EXCEPTION_HANDLER SystemHandler;
	PUSER_EXCEPTION_HANDLER FirstHandler;
	PUSER_EXCEPTION_HANDLER* ExceptionHandlers;
	WORD32 ExceptionHandlerCount;

	BOOLEAN IsSuspended;
	PSYSTEM_MEMORY_HEAP PrimaryThreadHeap;
	SYSTEM_TIME LastThreadCall;
}USER_THREAD, *PUSER_THREAD;

typedef struct _USER_LIBRARY { // User-mode Handle Information
	WORD64 LibraryId;
	PSYSTEM_HANDLE SystemHandle;
	STRING_PTR LibraryName;
	CHAR LibraryShortName[16];
	PUSER_PROCESS ParentProcess;

	PUSER_THREAD MainThread;
	PUSER_THREAD* CreatedThreads;
	WORD32 CreatedThreadCount;

	PMOTH_EXEC_EXPORTTABLE ExportTable;
	PMOTH_EXEC_IMPORTTABLE ImportTable;
}USER_LIBRARY, *PUSER_LIBRARY;

typedef struct _USER_ISC_PROCESS { // Inter-State Communication : Process
	WORD64 IscId;
	PSYSTEM_HANDLE SystemHandle;
	PUSER_PROCESS Creator;
	PUSER_PROCESS Target;

	WORD64 Message;
	WORD64 Arguments[4];
}USER_ISC_PROCESS, *PUSER_ISC_PROCESS;

typedef struct _USER_ISC_SESSION { // Inter-State Communication : Session
	WORD64 IscId;
	PSYSTEM_HANDLE SystemHandle;
	PUSER_SESSION Creator;
	PUSER_SESSION Target;

	WORD64 Message;
	WORD64 Arguments[4];
}USER_ISC_SESSION, *PUSER_ISC_SESSION;

typedef struct _USER_ISC_THREAD { // Inter-State Communication : Thread
	WORD64 IscId;
	PSYSTEM_HANDLE SystemHandle;
	PUSER_THREAD Creator;
	PUSER_THREAD Target;

	WORD64 Message;
	WORD64 Arguments[4];
}USER_ISC_THREAD, *PUSER_ISC_THREAD;

typedef struct _USER_ISC_LIBRARY { // Inter-State Communication : Library
	WORD64 IscId;
	PSYSTEM_HANDLE SystemHandle;
	PUSER_LIBRARY Creator;
	PUSER_LIBRARY Target;

	WORD64 Message;
	WORD64 Arguments[4];
}USER_ISC_LIBRARY, *PUSER_ISC_LIBRARY;

typedef struct _USER_HANDLE { // User-mode handle
	WORD64 HandleId;
	STRING_PTR HandleName;
	SYSTEM_HANDLE HandleValue;
	PSYSTEM_HANDLE SystemHandle;
	WORD32 OwnerType;
	struct {
		PUSER_LIBRARY Library;
		PUSER_PROCESS Process;
		PUSER_THREAD Thread;
		PUSER_SESSION Session;
	}Owners;

	WORD32 HandleType;
}USER_HANDLE, *PUSER_HANDLE;

// Kernel API Information

typedef struct _SYSTEM_VIDEO_FONT {

}SYSTEM_VIDEO_FONT, *PSYSTEM_VIDEO_FONT;

typedef struct _SYSTEM_VIDEO_COMMAND { // Kernel Video Manager : Command
	WORD16 VideoCommandType;
	WORD64 FrameNumber;

	union {
		WORD64 InternalArguments[4];

		struct {
			WORD32 X1, Y1, X2, Y2;
			WORD32 Color;
		}Line;

		struct {
			WORD32 X, Y, W, H;
			WORD32 Color;
		}Rectangle;

		struct {
			WORD32 X, Y, W, H;
			WORD32 Color;
		}Outline;

		struct {
			WORD32 X, Y;
			STRING_PTR String;
			PSYSTEM_VIDEO_FONT Font;
		}Text;

		struct {
			WORD32 X, Y, W, H;

			WORD32 ColorTopLeft;
			WORD32 ColorTopRight;
			WORD32 ColorBottomLeft;
			WORD32 ColorBottomRight;
		}Gradient;
	}SpecificArguments;
}SYSTEM_VIDEO_COMMAND, *PSYSTEM_VIDEO_COMMAND;

typedef struct _SYSTEM_VIDEO_CONTEXT { // Kernel Video Manager : Context
	WORD64 FrameCount;
	WORD32 TargetFramesPerSecond;

	PSYSTEM_VIDEO_COMMAND VideoCommandsForFrame;
	WORD32 VideoCommandsCount;
	WORD32 VideoCommandsMaxCount; 
}SYSTEM_VIDEO_CONTEXT, *PSYSTEM_VIDEO_CONTEXT;

#define SYSINPUT_MEARG_MOUSEPOSX  0x01
#define SYSINPUT_MEARG_MOUSEPOSY  0x02
#define SYSINPUT_MEARG_BUTTONUP   0x04
#define SYSINPUT_MEARG_BUTTONDOWN 0x08
#define SYSINPUT_MEARG_SCROLL     0x10
#define SYSINPUT_MEARG_RESPONSE   0x20

typedef struct _SYSTEM_INPUT_COMMAND { // Kernel Input Manager : Command
	WORD64 InputCount;
	WORD32 InputType;
	WORD32 DeviceId;
	PSYSTEM_DEVICE RootDevice;
	PSYSTEM_DEVICE_EVENT CausationEvent;

	union {
		WORD64 InternalArguments[8];

		struct {
			WORD16 ThisKey;
			WORD32 EventType;
			WORD16 EventArgument2;

			WORD64 CurrentKeyMap[4];
		}KeyboardEvent;

		struct {
			WORD16 EventParameters;

			WORD16 MouseX, MouseY;
			WORD16 ScrollPosition;
			WORD16 MouseButtons;
			WORD16 ScrollDelta;
		}MouseEvent;

		struct {
			WORD64 EventId;

			WORD64 EventArguments[7];
		}GenericEvent;
	}SpecificInputs;
}SYSTEM_INPUT_COMMAND, *PSYSTEM_INPUT_COMMAND;

typedef struct _SYSTEM_INPUT_CONTEXT { // Kernel Input Manager : Context
	WORD64 CommandCount;
	WORD64 ExecutedCommandCount;

	PSYSTEM_INPUT_COMMAND PendingCommands;
	WORD64 PendingCommandCount;
	WORD64 PendingCommandMax;

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
}SYSTEM_DEVICE_CONTROL, *PSYSTEM_DEVICE_CONTROL;

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