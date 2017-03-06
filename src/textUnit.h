#pragma once
#include <string>

class textUnit
{
public:
	textUnit()
		:_textZH("")
		,_textEN("")
	{}
	textUnit(std::string textZH, std::string textEN)
		:_textZH(textZH)
		,_textEN(textEN)
	{}

	std::string getText(bool isZH)
	{
		return isZH ? _textZH : _textEN;
	}
	void setText(std::string textZH, std::string textEN)
	{
		_textZH = textZH;
		_textEN = textEN;
	}
private:
	std::string _textZH, _textEN;
};