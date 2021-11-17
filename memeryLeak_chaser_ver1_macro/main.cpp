// memeryLeak_chaser_ver1_macro.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#pragma once

#include "stdafx.h"

#include <stdio.h>
#include <iostream>
#include <typeinfo>
#include <typeindex>

#include "memoryLeak.h"


using namespace std;


//test code
int main() {

	int *b = NULL;
	int *c = NULL;
	int *d = NULL;

	b = _MALLOC(int, 5);
	b[0] = 0;
	b[1] = 1;
	b[2] = 2;
	b[3] = 3;
	b[4] = 4;

	c = _MALLOC(int, 10);

	d= _MALLOC(int, 20);


	_DELETE(b);
	_DELETE(c);

	//g_MemoryLeak.DisplayMallocStatics();

	//g_MemoryLeak.DisplayAllRelease();


	g_MemoryLeak.DisplayAllStatics();
	
	while (true)
	{

	}

}
