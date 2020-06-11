#include "GameSettings.h"

GameSettings::GameSettings(const std::string s)
{
	std::ifstream in(s);
	for (std::string str; std::getline(in, str);)
	{
		if (str == "[Tile Size]")
		{
			in >> dimension;
		}
		else if (str == "[Board Size]")
		{
			in >> width >> height;
		}
		else if (str == "[Speedup Rate]")
		{
			in >> speedupFactor;
		}
		else if (str == "[Poison Amount]")
		{
			in >> nPoison;
		}
		else if (str == "[Goal Amount]")
		{
			in >> nFood;
		}
	}
}

int GameSettings::GetFoodNum() const
{
	return nFood;
}

int GameSettings::GetPoisonNum() const
{
	return nPoison;
}

int GameSettings::GetWidth() const
{
	return width;
}

int GameSettings::GetHeight() const
{
	return height;
}

float GameSettings::GetSpeedupFactor() const
{
	return speedupFactor;
}

int GameSettings::GetDimension() const
{
	return dimension;
}
