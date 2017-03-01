#pragma once

#include <string>
using namespace std;

#define CREATE_VALUE(NAME) static const string NAME = #NAME;
#define CREATE_VALUE_WITH_VALUE(NAME, VALUE) static const string NAME = VALUE;

namespace NAME_MGR
{
	//Image
	CREATE_VALUE(I_Gradient);

	CREATE_VALUE(I_BtnZH1);
	CREATE_VALUE(I_BtnEN1);
	CREATE_VALUE(I_BtnZH2);
	CREATE_VALUE(I_BtnEN2);
	CREATE_VALUE(I_BtnZH3);
	CREATE_VALUE(I_BtnEN3);
	CREATE_VALUE(I_BtnZH4);
	CREATE_VALUE(I_BtnEN4);

	//Audio
	CREATE_VALUE(BTN_CLICK);
	CREATE_VALUE(BGM);
}