#pragma once

#include <string>
using namespace std;

#define CREATE_VALUE(NAME) static const string NAME = #NAME;
#define CREATE_VALUE_WITH_VALUE(NAME, VALUE) static const string NAME = VALUE;

namespace NAME_MGR
{
	CREATE_VALUE(I_Gradient);

	CREATE_VALUE(BGM_1);
	CREATE_VALUE(BGM_2);
}