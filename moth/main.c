/*
main.c
moth
15 November 2023

The Moth Operating Systemsup
(c) Noah Wooten 2023, All Rights Reserved

This file is licensed under the MothOSL.
https://github.com/noahw2021/moth/blob/master/LICENSE

Changes:
11/15/23 : File created - nw
*/

#include "include/mot.h"
#if (MOTH_PLATFORM == MOTH_PLATFORM_AMD64CCEL)
#include "include/ccel.h"
#ifdef _DEBUG
#else
#endif
#endif

int main(void) {
#if (MOTH_PLATFORM == MOTH_PLATFORM_AMD64CCEL)
	CcInit();
	*(int*)(0x47981) = 69;
#endif
}