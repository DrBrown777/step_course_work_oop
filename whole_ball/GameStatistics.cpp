#include "GameStatistics.h"

string GameStatistics::ToString(int n, int d)
{
	ostringstream out;
	out << setw(d);
	out << setfill('0') << n;
		
	return out.str();
}

GameStatistics::GameStatistics()
{
	score_i = 0; energy_i = 0;
	live_i = 3; timer_f = 0;

	font.loadFromFile("consola.ttf");

	score = make_pair(score_t, score_d);
	score.first.setString("Score");
	score.second.setString("00000");

	timer = make_pair(timer_t, timer_d);
	timer.first.setString("Time");
	timer.second.setString("000");

	live = make_pair(live_t, live_d);
	live.first.setString("Live");
	live.second.setString("0");

	energy = make_pair(energy_t, energy_d);
	energy.first.setString("Energy");
	energy.second.setString("00");

	for (auto it : { &score, &timer, &live, &energy })
	{
		it->first.setFont(font);
		it->second.setFont(font);
		it->first.setCharacterSize(30);
		it->second.setCharacterSize(25);
		it->first.setFillColor(Color(30, 144, 225));
		it->second.setFillColor(Color(255, 140, 0));
	}

	score.first.setPosition(180, 0);
	score.second.setPosition(190, 30);

	energy.first.setPosition(380, 0);
	energy.second.setPosition(415, 30);

	timer.first.setPosition(580, 0);
	timer.second.setPosition(590, 30);

	live.first.setPosition(760, 0);
	live.second.setPosition(785, 30);
}

void GameStatistics::Update(const float& second, const int& countEnergyPills)
{
	timer_f = timeRound - second;
	timer.second.setString(ToString(timer_f*10, 4));

	score.second.setString(ToString(score_i, 5));
	
	energy_i = countEnergyPills;
	energy.second.setString(ToString(energy_i, 2));

	live.second.setString(ToString(live_i, 1));
}

void GameStatistics::SetLiveDown()
{
	live_i--;
}

void GameStatistics::SetTimeRonud()
{
	timer_f = timeRound;
}

void GameStatistics::SetScoreUp()
{
	score_i += pointScore;
}

int GameStatistics::GetTimeRound()
{
	return timer_f;
}

int GameStatistics::GetLive()
{
	return live_i;
}

void GameStatistics::DrawStatistics(RenderWindow& window)
{
	for (auto it : { &score, &timer, &live, &energy })
	{
		window.draw(it->first);
		window.draw(it->second);
	}
}
