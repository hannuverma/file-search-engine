#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
// Function to create and return an ofstream (write mode)


std::string changeToLowerCase(const std::string input); // these are used in other functions hence its important to define them here
std::ofstream deleteData(const std::string& fileName);
bool checkFile(std::string fileName);


std::ifstream writeSettings(const std::string& fileName, int settingChange, bool caseSenstive, bool wholeWord, bool showWholePath) // to change the setting
{
	std::ifstream inFile(fileName);

	const int noOfSettings{3}; 

	std::string line;
	std::string file[noOfSettings]; // if the array was larger than needed than that would lead to empty lines in the txt file 
	int i{};

	if (!checkFile(fileName))
	{
		return inFile;
	}

	while (getline(inFile, line)) // filling array with each line from txt file
	{
		file[i] = line;
		i++;
	}

	if (settingChange == 1) 
	{
		if (caseSenstive)
		{
			file[0] = "case - insensitve";
		}
		else
		{
			file[0] = "case - sensitive";
		}
	}

	else if (settingChange == 2)
	{

		if (wholeWord)
		{
			file[1] = "whole word - false";
		}
		else
		{
			file[1] = "whole word - true";
		}


	}
	else if (settingChange == 3)
	{
		if (showWholePath)
		{
			file[2] = "show full path - no"; 
		}
		else
		{
			file[2] = "show full path - yes";
		}
	}

	else
	{
		inFile.close();
		return inFile;
	}

	inFile.close();

	std::ofstream outFile(fileName);

	deleteData(fileName);

	for (int i{}; i < noOfSettings; i++) //writing array back into the txt file
	{
		outFile << file[i] << '\n';
	}

	outFile.close();
	return inFile;
}

// Function to create and return an ifstream (read mode)
std::ifstream readFile(const std::string& fileName)
{
	int lineNumber{ 1 };
	std::string myText;

	std::ifstream inFile(fileName); // Open the file for reading
	if (!checkFile(fileName))
	{
	}

	while (std::getline(inFile, myText))
	{
		std::cout << '\n' << lineNumber++ << ": " << myText << '\n';
	}

	inFile.close();

	return inFile;
}

bool checkFile(std::string fileName) // just a function to tell if the file was opened successfully 
{
	std::ifstream inFile(fileName);

	if (!inFile.is_open())
	{
		std::cerr << "Error opening file: " << fileName << '\n';

		return false;
	}
	inFile.close();
	return true;
}

std::ifstream searchFile( std::string fileName, std::string keyword, bool caseSenstive, bool wholeWord, int& totalMatches, bool showFullPath)
{
	std::ifstream inFile(fileName);
	std::string line;
	std::string fileNameT;
	std::string keyword2;

	fileNameT = fileName;

	bool found = false;
	int i{};
	totalMatches = 0;

	if (!checkFile(fileName))
	{
		return inFile;
	}
	if(showFullPath)
		std::cout << "The following instances of \033[34m" << keyword << "\033[0m were found in the file \033[34m" << fileName <<"\033[0m\n";

	else
	{
		while (true) {
			size_t pos = fileNameT.find("\\");
			if (pos == std::string::npos) break; // stop safely
			fileNameT.erase(0, pos + 1);
		}
		std::cout << "The following instances of \033[34m" << keyword << "\033[0m were found in the file \033[34m" << fileNameT << "\033[0m\n";
	}

	while (getline(inFile, line))
	{
		i++; // to tell which line in the txt file the keyword was found


		if (caseSenstive) 
		{

			if (line.find(keyword) != std::string::npos)
			{


				size_t pos = line.find(keyword); // the position of the keyword

				if (wholeWord && pos == 0) 
				{
					keyword = keyword + " ";
				}
				else if (wholeWord && pos != 0)
				{
					keyword = " " + keyword + " ";
				}

				std::cout << i << ". ";

				std::cout << line.substr(0, pos); // break the line into three parts - before the keyword

				std::cout << "\033[31m" << keyword << "\033[0m"; // the keyword that is red and underlined

				std::cout << line.substr(pos + keyword.length()) << '\n'; // after the keyword

				totalMatches++;
				found = true;
			}
		}
		else
		{
			keyword2 = changeToLowerCase(keyword); // as its case insensitive we changed everything to lower case 


			if (changeToLowerCase(line).find(keyword2) != std::string::npos)
			{

				size_t pos = changeToLowerCase(line).find(keyword2);
				if (pos != std::string::npos)
				{
					bool valid = true;

					// Check char before
					if (pos > 0 && std::isalnum(static_cast<unsigned char>(line[pos - 1]))) // checking if the char before and after the keyword is alphanumeric to tell if its a whole word or not
					{
						valid = false;
					}

					// Check char after
					if (pos + keyword.length() < line.length() &&
						std::isalnum(static_cast<unsigned char>(line[pos + keyword.length()])))
					{
						valid = false;
					}

					if (wholeWord && !valid) // this takes all the cases where the matching word wont be considered 
					{
					}
					else
					{
						std::cout << i << ". ";
						std::cout << line.substr(0, pos);
						std::cout << "\033[31;4m" << line.substr(pos, keyword.length()) << "\033[0m";
						std::cout << line.substr(pos + keyword.length()) << '\n';

						totalMatches++;
						found = true;
					}
				}
			}
		}


	}



	if (!found)
	{
		std::cout << keyword << " not found in the file: " << fileNameT << '\n';
	}

	std::cout << "Total " << "\033[32m" << totalMatches << "\033[0m" << " matches\n";

	inFile.close();



	return inFile;
}

std::ifstream settings(const std::string& fileName, bool& caseSenstive, bool& wholeWord, bool& showFullPath)
{
	std::string line;
	std::ifstream file(fileName);

	caseSenstive = false;
	wholeWord = true;
	showFullPath = false;

	if (!checkFile(fileName))
	{

		return file;
	}



	while (getline(file, line)) // its quite clever (and lazy)
	{


		if (line.find("sensitive") != std::string::npos)
		{
			caseSenstive = true;
		}

		if (line.find("false") != std::string::npos)
		{
			wholeWord = false;
		}
		
		if (line.find("yes") != std::string::npos)
		{
			showFullPath = true;
		}

	}



	file.close();
	return file;
}

void extractHistoryEntry(const std::string& line, std::string& keyword, std::string& filename, int& totalMatches) {
	std::stringstream ss(line);
	std::string temp;

	if (std::getline(ss, keyword, '-') &&
		std::getline(ss, filename, '-') &&
		std::getline(ss, temp)) {

		// Trim spaces
		auto trim = [](std::string& s) {
			s.erase(0, s.find_first_not_of(" "));
			s.erase(s.find_last_not_of(" ") + 1);
			};

		trim(keyword);
		trim(filename);
		trim(temp);

		size_t eqPos = temp.find('=');
		if (eqPos != std::string::npos)
		{
			std::string numberStr = temp.substr(eqPos + 1);
			trim(numberStr);
			try 
			{
				totalMatches = std::stoi(numberStr);
			}
			catch (...) 
			{
				totalMatches = 0; // fallback if conversion fails
			}
		}
		else 
		{
			totalMatches = 0; // fallback if '=' not found
		}
	}

	
}
std::ofstream writeHistory(std::string& fileName, std::string sHistoryKeyword, std::string sHistoryFile, int aTotalMatches)
{
	std::ofstream file(fileName, std::ios::app);

	checkFile(fileName);

	file << sHistoryFile << " - " << sHistoryKeyword << " - total matches = " << aTotalMatches << '\n';

	file.close();
	return file;
}

std::ofstream deleteData(const std::string& fileName)
{

	std::ofstream file(fileName);

	if (!checkFile(fileName))
	{
		return file;
	}

	file.close();
	return file;
}

bool FileIsEmpty(const std::string& fileName)
{
	std::ifstream file(fileName);
	return file.peek() == std::ifstream::traits_type::eof();
}