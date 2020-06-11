#pragma once
#include <fstream>
#include <string>
class GameSettings {
public:
	GameSettings(const std::string s);
	int GetFoodNum() const;
	int GetPoisonNum() const;
	int GetWidth() const;
	int GetHeight() const;
	float GetSpeedupFactor() const;
	int GetDimension() const;
private:
	int nFood;
	int nPoison;
	int width;
	int height;
	float speedupFactor;
	int dimension;
};