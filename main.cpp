#include <iostream>
#include <string>
#include <fstream>
#include <cctype> 
#include <algorithm>
#include <vector>

using namespace std;

const string settingFile = "settings.txt";
string historyFile = "history.txt";


std::ifstream searchFile(std::string fileName, std::string keyword, bool caseSenstive, bool wholeWord, int& totalMatches, bool showFullPath);
std::ifstream settings(const std::string& fileName, bool& caseSenstive, bool& wholeWord, bool& logicAnd);
std::ifstream readFile(const std::string& fileName);
std::ifstream writeSettings(const std::string& fileName, int settingChange, bool caseSenstive, bool wholeWord, bool logicAnd);
bool checkFile(std::string fileName);
bool FileIsEmpty(const std::string& fileName);
std::ofstream deleteData(const std::string& fileName);
std::ofstream writeHistory(std::string& fileName, std::string sHistoryKeyword, std::string sHistoryFile, int aTotalMatches);
void extractHistoryEntry(const std::string& line, std::string& keyword, std::string& filename, int& totalMatches);


std::string trimQuotes(const std::string& path) // if user accidently put the address between quotes because thats how it showed in properties 
{
	string path2;
	path2 = path;
	if (!path.empty() && path.front() == '"' && path.back() == '"')
	{
		path2 = path.substr(1, path.size() - 2);
	}
	else if (!path.empty() && path.front() == '"')
	{
		path2 = path.substr(1, path.size());
	}
	else if (!path.empty() && path.back() == '"')
	{
		path2 = path.substr(0, path.size() - 1);
	}
	return path2;
}


string changeToLowerCase(const string input) // this is being used when the setting is case insensitive we basically change everything to lowercase 
{
	string lineL; // line that will be lower case
	lineL = input; // original line


	for (int l{}; l <= input.length(); l++)
	{
		lineL[l] = tolower(input[l]);
	}

	return lineL;
}

int main()
{
	string IFile;  // the input file which is being searched
	string keyword; // the keyword that is being searched
	bool caseSenstive = false; 
	int settingChange{}; // which setting needs to be changed
	bool wholeWord = false; 
	bool showFullPath = false;

	const int maxHistory{ 100 };
	string sHistoryKeyword[maxHistory]; // keeping history of keywords
	string sHistoryFile[maxHistory]; // keeping history of file names
	int historyNumber{}; // keeping track of the number of histories taken
	int totalMatches{}; 
	int aTotalMatches[maxHistory]{};
	int numberOfFiles{};
	int grandTotalMatches{};

	vector<string> allFiles;

	ifstream file(historyFile);
	string line;
	while (getline(file, line) && historyNumber < maxHistory)
	{
		extractHistoryEntry(line, sHistoryKeyword[historyNumber], sHistoryFile[historyNumber], aTotalMatches[historyNumber]);
		historyNumber++;
	}

	


	while (true)
	{
		grandTotalMatches = 0;
		numberOfFiles = 1;
		allFiles.clear();

		settings(settingFile, caseSenstive, wholeWord, showFullPath); // checking the settings 


		while (true)
		{
			
			cout << "Enter the full address of the file: ";
			std::getline(cin >> ws, IFile); // inputing the file address

			if (IFile == "" || IFile == " " || changeToLowerCase(IFile) == "done" || changeToLowerCase(IFile) == "setting" || changeToLowerCase(IFile) == "settings" || changeToLowerCase(IFile) == "s" || changeToLowerCase(IFile) == "history" || changeToLowerCase(IFile) == "h")
			{
				break;
			}
			else
			{
				if(checkFile(trimQuotes(IFile)))
					allFiles.push_back(trimQuotes(IFile));
			}

			numberOfFiles++;

		}


		if (changeToLowerCase(IFile) == "setting" || changeToLowerCase(IFile) == "settings" || changeToLowerCase(IFile) == "s")
		{
			{
				readFile(settingFile);

				cout << "Enter 1 to change the case settings \n";
				cout << "Enter 2 to change the whole word search settings \n";
				cout << "Enter 3 to change show full path setting \n";
				cout << "Enter 9 to exit \n";
				cin >> settingChange;


				writeSettings(settingFile, settingChange, caseSenstive, wholeWord, showFullPath);
			}
			readFile(settingFile);


		}

		else if ((changeToLowerCase(IFile) == "history" || changeToLowerCase(IFile) == "h") && FileIsEmpty)
		{
			string historyLine{};

			for (int i{}; i < historyNumber; i++) // to output the different strings we have in the arrays for history 
			{
				cout << '\n' << i + 1 << ": " << sHistoryKeyword[i] << " - " << sHistoryFile[i] << " - total matches = " << aTotalMatches[i] << '\n';
			}

			cout << "Enter the line number which you want to re-search or enter escape or e to exit: ";
			cin >> historyLine;
			int historyL{};

			if (!historyLine.empty() && std::all_of(historyLine.begin(), historyLine.end(), ::isdigit)) // checking if input is a numerical value
			{
				historyL = stoi(historyLine);
				searchFile(sHistoryFile[historyL - 1], sHistoryKeyword[historyL - 1], caseSenstive, wholeWord, totalMatches, showFullPath); // -1 because arrays start from 0 so if the user had entered 3 than the array value should be 2
			}

		}


		else // checking if file opened without any problems
		{

			cout << "Enter the keyword to find: ";
			std::getline(cin >> ws, keyword); // input for the keyword to search

			for (int i{}; i < allFiles.size(); i++)
			{
				searchFile(allFiles[i], keyword, caseSenstive, wholeWord, totalMatches, showFullPath); // searching the file while removing the quotes and checking the settings 

				sHistoryFile[historyNumber] = allFiles[i]; // saving the history 
				sHistoryKeyword[historyNumber] = keyword;
				aTotalMatches[historyNumber] = totalMatches;

				historyNumber++;

				grandTotalMatches += totalMatches;

				writeHistory(historyFile, allFiles[i], keyword, totalMatches);
			}

			cout << "In total \033[1;36m" << grandTotalMatches << "\033[0m matches were found\n";

		}
	}

}

