#pragma once
#include <SFML/Graphics.hpp>
#include <iomanip>
#include <sstream>

using namespace sf;
using namespace std;

class GameStatistics
{
private:
	Font font;

	Text score_t, score_d;
	Text timer_t, timer_d;
	Text live_t, live_d;
	Text energy_t, energy_d;

	pair <Text, Text> score;
	pair <Text, Text> timer;
	pair <Text, Text> live;
	pair <Text, Text> energy;

	int score_i, live_i, energy_i;
	float timer_f;

	const float timeRound = 100;
	const int pointScore = 25;

	string ToString(int n, int d);

public:
	GameStatistics();
	void Update(const float& second, const int& countEnergyPills);
	void SetLive();
	int GetTimeRound();
	void DrawStatistics(RenderWindow& window);
};

