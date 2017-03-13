#pragma once
#include <string>

class textUnit
{
public:
	textUnit()
		:_textZH("")
		,_nameBigEN("")
	{}
	textUnit(std::string textZH, std::string textEN)
		:_textZH(textZH)
		,_nameBigEN(textEN)
	{}

	std::string getText(bool isZH)
	{
		return isZH ? _textZH : _nameBigEN;
	}
	void setText(std::string textZH, std::string textEN)
	{
		_textZH = textZH;
		_nameBigEN = textEN;
	}
private:
	std::string _textZH, _nameBigEN;
};