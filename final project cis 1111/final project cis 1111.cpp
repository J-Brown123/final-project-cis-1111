// Jordan Brown CIS 1111.800
// jordan.brown@sinclair.edu
// July 12, 2022
// Final Project

#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <fstream>

using namespace std;

//Constants
const int ELO_WEIGHT = 3; //First three all for elo calculations
const double MAX_ELO = 3000.00;
const int K_FACTOR = 24;
const int INT_COMPONENTS = 3; //For final ranking calculations and array size
const int RAT_COMPONENTS = 2;
const int NUM_TEAMS = 10;

string teamSymbols[NUM_TEAMS] = { "UCF", "CIN", "ECU", "HOU", "MEM", "USF", "SMU", "TEM", "TLN", "TUL" };

//2D Arrays for data storage
int teamData[NUM_TEAMS][INT_COMPONENTS] = { {0, 0, 1500}, //Each team's wins (0), losses (1), current elo (2)
										{0, 0, 1500}, //Elo starts at 1500 (average)
										{0, 0, 1500}, //Can be updated from text file
										{0, 0, 1500},
										{0, 0, 1500},
										{0, 0, 1500},
										{0, 0, 1500},
										{0, 0, 1500},
										{0, 0, 1500},
										{0, 0, 1500} };
double processedData[NUM_TEAMS][RAT_COMPONENTS] = { {0, 0.5}, //Data taken from the int array 
													{0, 0.5}, //converted into ratio form
													{0, 0.5}, //for final rankings
													{0, 0.5}, //win percentage (0)
													{0, 0.5}, //strength of schedule (1)
													{0, 0.5}, //elo placed on 0-1 scale (2)
													{0, 0.5},
													{0, 0.5},
													{0, 0.5},
													{0, 0.5} };

//For updating data after scores are inputted
void ComponentUpdate (int teamA, int teamB, int teamAResult, int teamBResult, int a, int b) //ELO ratings for each team, which team won, subscripts
{
	//Expected win chance
	double teamAExpected;
	double teamBExpected;
	teamAExpected = 1.00 / (1 + (pow(10.00, (teamB - teamA) / 400.00)));
	teamBExpected = 1.00 / (1 + (pow(10.00, (teamA - teamB) / 400.00)));

	//Updating elo in array
	teamData[a][2] = teamData[a][2] + K_FACTOR * (teamAResult - teamAExpected);
	if (teamData[a][2] > MAX_ELO)
		teamData[a][2] = MAX_ELO;
	teamData[b][2] = teamData[b][2] + K_FACTOR * (teamBResult - teamBExpected);
	if (teamData[b][2] > MAX_ELO)
		teamData[b][2] = MAX_ELO;

	//Update win percentage, elo in processedData
	double wins = teamData[a][0];
	double losses = teamData[a][1];
	processedData[a][0] = wins / (wins + losses);
	processedData[a][1] = teamData[a][2] / MAX_ELO;

	wins = teamData[b][0];
	losses = teamData[b][1];
	processedData[b][0] = wins / (wins + losses);
	processedData[b][1] = teamData[b][2] / MAX_ELO;

	return;
}

void DisplayTeams()
{
	for (int i = 0; i < NUM_TEAMS; i++)
		cout << i + 1 << "-" << teamSymbols[i] << "  ";
	cout << endl << endl;
	return;
}

//Input data each week of season
void InputScores()
{
	int scoresTrue;
	int teamA;
	int teamB;
	int teamAResult;
	int teamBResult;
	int gameResult;
	cout << "Are there scores to input?\n1 - YES\n2 - NO\n";
	cin >> scoresTrue;
	while (scoresTrue != 1 && scoresTrue != 2)
	{
		cout << "INVALID RESPONSE\n1 - YES\n2 - NO\n";
		cin >> scoresTrue;
	}
	while (scoresTrue == 1)
	{
		//Display the teams available
		DisplayTeams();

		//Choose teams
		cout << "Choose the first team:\n";
		cin >> teamA;
		while (teamA <= 0 || teamA > NUM_TEAMS)
		{
			cout << "INVALID RESPONSE\n";
			cout << "Choose the first team:\n";
			cin >> teamA;
		}
		cout << "Choose the second team:\n";
		cin >> teamB;
		while (teamB <= 0 || teamB > NUM_TEAMS)
		{
			cout << "INVALID RESPONSE\n";
			cout << "Choose the second team:\n";
			cin >> teamB;
		}
		while (teamB == teamA)
		{
			cout << "The teams must be different.\n";
			cout << "Please choose the second team.\n";
			cin >> teamB;
		}

		//Find out winner
		cout << "Which team won the game?\n1 - " << teamSymbols[teamA - 1] << endl
			<< "2 - " << teamSymbols[teamB - 1] << endl;
		cin >> gameResult;
		while (gameResult != 1 && gameResult != 2)
		{
			cout << "INVALID RESPONSE\n";
			cout << "Which team won the game?\n1 - " << teamSymbols[teamA - 1] << endl
				<< "2 - " << teamSymbols[teamB - 1] << endl;
			cin >> gameResult;
		}

		//Update each team's record
		if (gameResult == 1)
		{
			teamData[teamA - 1][0] += 1;
			teamData[teamB - 1][1] += 1;
			teamAResult = 1;
			teamBResult = 0;
		}
		else if (gameResult == 2)
		{
			teamData[teamB - 1][0] += 1;
			teamData[teamA - 1][1] += 1;
			teamAResult = 0;
			teamBResult = 1;
		}


		//Update data as result of game
		ComponentUpdate(teamData[teamA - 1][2], teamData[teamB - 1][2], teamAResult, teamBResult, teamA - 1, teamB - 1);

		cout << "Are there more scores to input?\n1 - YES\n2 - NO\n";
		cin >> scoresTrue;
		while (scoresTrue != 1 && scoresTrue != 2)
		{
			cout << "INVALID RESPONSE\n1 - YES\n2 - NO\n";
			cin >> scoresTrue;
		}
	}
	cout << "Thank you!\n\n";
	return;
}

//Display all components of all teams
void Components()
{
	cout << "TEAM    WINS    LOSSES    WIN PCT    ELO\n";
	for (int i = 0; i < NUM_TEAMS; i++)
	{
		double wins = teamData[i][0];
		double losses = teamData[i][1];
		cout << right << setw(4) << teamSymbols[i] << setw(8) << wins << setw(10) << losses;
		if ((wins + losses) == 0)
			cout << setw(11) << "0.000";
		else
			cout << setw(11) << wins / (wins + losses);
		cout << setw(7) << setw(7) << teamData[i][2] << endl << endl;
	}
	return;
}

//Choose one component of one team to view
double Components(int i)
{
	int selection;
	cout << "Which component for " << teamSymbols[i] << " would you like to view?\n";
	cout << "1-Win Percentage 2-ELO\n";
	cin >> selection;
	while (selection < 1 || selection > 3)
	{
		cout << "INVALID RESPONSE\n";
		cout << "1-Win Percentage 2-ELO\n";
		cin >> selection;
	}
	switch (selection)
	{
	case 1: return processedData[i][0];
		break;
	case 2: return teamData[i][2];
		break;
	}
	cout << endl << endl;
	return -1;
}

//Clear data at season end, start fresh next season
void SeasonEnd()
{
	int choice;
	cout << "Are you sure you want to adjust the stats for season end?\n1- YES\n2- NO\n";
	cin >> choice;
	while (choice != 1 && choice != 2)
	{
		cout << "INVALID RESPONSE\n1- YES\n2- NO\n";
		cin >> choice;
	}
	if (choice == 2)
		return;
	if (choice == 1)
	{
		//Bring elo halfway to average to account for roster change
		for (int i = 0; i < NUM_TEAMS; i++)
		{
			teamData[i][2] = (teamData[i][2] + 1500) / 2;
			processedData[i][1] = teamData[i][2] / MAX_ELO;
		}

		//Set wins and losses to zero
		for (int i = 0; i < NUM_TEAMS; i++)
		{
			for (int j = 0; j < INT_COMPONENTS - 1; j++)
				teamData[i][j] = 0;
		}

		//Set win percentage to zero
		for (int i = 0; i < NUM_TEAMS; i++)
		{
			processedData[i][0] = 0.0;
		}

		cout << "Adjustments complete.\n";
	}
	return;
}

//Store data in txt file
void SaveData()
{
	//Create Output File
	string filename;
	cout << "Enter the desired file name in the following form:\n";
	cout << "[SeasonYear]Week[Number].txt\n";
	cin >> filename;
	ofstream outFile;
	outFile.open(filename);

	//Store Data in File
	for (int i = 0; i < NUM_TEAMS; i++)
	{
		for (int j = 0; j < INT_COMPONENTS; j++)
		{
			outFile << teamData[i][j] << " ";
		}
	}
	for (int i = 0; i < NUM_TEAMS; i++)
	{
		for (int j = 0; j < RAT_COMPONENTS; j++)
		{
			outFile << processedData[i][j] << " ";
		}
	}
	outFile.close();
	cout << endl;
	return;
}

//Get data from file
void UpdateData()
{
	//Create Output File
	string filename;
	cout << "Enter the file to look for in the following form:\n";
	cout << "[SeasonYear]Week[Number].txt\n";
	cin >> filename;
	ifstream inFile;
	inFile.open(filename);
	cout << endl;

	//Validation
	while (!inFile)
	{
		cout << "File does not exist\n";
		cout << "Enter the file to look for in the following form:\n";
		cout << "[SeasonYear]Week[Number].txt\n";
		cout << "Enter 0 to exit.\n";
		cin >> filename;
		if (filename == "0")
			return;
		inFile.open(filename);
	}
	//Store Data in File
	for (int i = 0; i < NUM_TEAMS; i++)
	{
		for (int j = 0; j < INT_COMPONENTS; j++)
		{
			inFile >> teamData[i][j];
		}
	}
	for (int i = 0; i < NUM_TEAMS; i++)
	{
		for (int j = 0; j < RAT_COMPONENTS; j++)
		{
			inFile >> processedData[i][j];
		}
	}
	cout << "Data has been updated.\n\n";
	inFile.close();
	return;
}

//Calculate and display final rankings
void Ranking()
{
	//Create array with final ranking score
	double ranking[NUM_TEAMS];

	//Create copy of symbols array
	string symbolsCopy[NUM_TEAMS] = { "UCF", "CIN", "ECU", "HOU", "MEM", "USF", "SMU", "TEM", "TLN", "TUL" };

	for (int i = 0; i < NUM_TEAMS; i++)
		ranking[i] = (processedData[i][0] + processedData[i][1] * ELO_WEIGHT) / (ELO_WEIGHT + RAT_COMPONENTS - 1); //Minus 1 to not count elo an extra time

	//Sort array so it is descending
	int max;
	int i;
	for (max = NUM_TEAMS - 1; max > 0; max--)
	{
		for (i = 0; i < max; i++)
		{
			if (ranking[i] < ranking[i + 1])
			{
				swap(ranking[i], ranking[i + 1]);
				swap(symbolsCopy[i], symbolsCopy[i + 1]);
			}
		}
	}

	//Display results
	for (int i = 0; i < NUM_TEAMS; i++)
		cout << left << setw(4) << i + 1 << "- " << setw(5) << symbolsCopy[i] << setw(6) << ranking[i] << endl << endl;

	return;
}


int main()
{
	int selection;
	cout << showpoint << fixed << setprecision(3);
	cout << "ULTRA COLLEGE RANKER\n";
	
	//Main Menu
	do
	{
		cout << "Choose from the following options:\n";
		cout << "1- DISPLAY TEAMS\n2- INPUT SCORES\n3- VIEW COMPONENTS\n";
		cout << "4- UPDATE DATA\n5- SAVE DATA\n6- CURRENT RANKINGS\n";
		cout << "7- SEASON ENDING ADJUSTMENT\n8- EXIT\n";
		cin >> selection;
		while (selection < 1 || selection > 8)
		{
			cout << "INVALID RESPONSE";
			cout << "Choose from the following options:\n";
			cout << "1- DISPLAY TEAMS\n2- INPUT SCORES\n3- VIEW COMPONENTS\n";
			cout << "4- UPDATE DATA\n5- SAVE DATA\n6- CURRENT RANKINGS\n";
			cout << "7- SEASON ENDING ADJUSTMENT\n8- EXIT\n";
		}
		switch (selection)
		{
		case 1: DisplayTeams();
			break;
		case 2: InputScores();
			break;
		case 3: int compChoice, teamChoice;
			cout << "Do you want to view all components, or just one from a certain team?\n";
			cout << "1- ALL\n2- CHOOSE\n";
			cin >> compChoice;
			while (compChoice != 1 && compChoice != 2)
			{
				cout << "INVALID RESPONSE\n";
				cout << "1- ALL\n2- CHOOSE\n";
				cin >> compChoice;
			}
			if (compChoice == 1)
				Components();
			if (compChoice == 2)
			{
				cout << "Please choose a team:\n";
				DisplayTeams();
				cin >> teamChoice;
				while (teamChoice < 1 || teamChoice > NUM_TEAMS)
				{
					cout << "INVALID RESPONSE\n";
					cout << "Please choose a team:\n";
					DisplayTeams();
					cin >> teamChoice;
				}
				cout << Components(teamChoice - 1) << endl << endl;
			}
			break;
		case 4: UpdateData();
			break;
		case 5: SaveData();
			break;
		case 6: Ranking();
			break;
		case 7: SeasonEnd();
			cout << endl;
			break;
		case 8: cout << "Thank you";
			return 0;
			break;
		}
	} while (selection != 8);
	return 0;
}

