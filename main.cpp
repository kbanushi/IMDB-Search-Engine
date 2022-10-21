/**********************************************************
 * HEADER
 **********************************************************/

#include <iostream>   
#include <cctype>     
#include <vector>     
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;


class TitleRecord{
public:
    void setInfo(const vector<string>& items); // Function used to set all the info of the object
    string getTitleID() const {return titleID;}
    string getPrimaryTitle() const {return primaryTitle;}
    string getStartYear() const {return startYear;}
    string getGenres() const {return genres;}
private:
    string titleID, primaryTitle, startYear, genres; //INDEX: 0, 2, 5, 8
};

void TitleRecord::setInfo(const vector<std::string> &items) {
    this->titleID = items.at(0);
    this->primaryTitle = items.at(2);
    this->startYear = items.at(5);
    this->genres = items.at(8);
}


/*********************************************** Helper functions ***************************************/
void splitString(string line, char delimiter, vector<string>& items);


//The splitString function takes a line and split it on the delimiter and push the parts into items
void splitString(string line, char delimiter, vector<string>& items) {
	stringstream s_stream(line);
	while (s_stream.good()) {
		string item;
		getline(s_stream, item, delimiter);
		items.push_back(item);
	}
}

class NameRecord{
public:
    void setInfo(const vector<string>& items);
    string getNameID() const {return nameID;}
    string getPrimaryName() const {return primaryName;}
    string getBirthYear() const {return birthYear;}
    string getDeathYear() const {return deathYear;}
    string getPrimaryProfession() const {return primaryProfession;}
private:
    string nameID, primaryName, birthYear, deathYear, primaryProfession; //INDEXES: 0, 1, 2, 3, 4
};

void NameRecord::setInfo(const vector<std::string> &items) {
    this->nameID = items.at(0);
    this->primaryName = items.at(1);
    this->birthYear = items.at(2);
    this->deathYear = items.at(3);
    this->primaryProfession = items.at(4);
}

class PrincipalRecord{
public:
    void setInfo(const vector<string>& items);
    string getTitleID() const {return titleID;}
    string getNameID() const {return nameID;}
    string getCharacters() const {return characters;}
private:
    string titleID, nameID, characters;
};

void PrincipalRecord::setInfo(const vector<std::string> &items) {
    this->titleID = items.at(0);
    this->nameID = items.at(2);
    this->characters = items.at(5);
}

/**********************************************************************************************************/

void fillMovieTitles(vector<TitleRecord>& titles, const string& titlesFile){
    TitleRecord temp;
    vector<string> items;

    ifstream wordFile(titlesFile);
    string tempString; //Used to read and append line to vector
    getline(wordFile,tempString); //First line is not necessary
    if(wordFile.is_open()){ //Check if the file has been opened first
        while(getline(wordFile,tempString)) {  //While there are still lines to read
            splitString(tempString, '\t', items);
            temp.setInfo(items);
            items.erase(items.begin(), items.end());
            titles.push_back(temp);
        }
    }

    wordFile.close(); //Close file
}

void fillNameRecords(vector<NameRecord>& names, const string& namesFile){
    NameRecord temp;
    vector<string> items;

    ifstream wordFile(namesFile);
    string tempString;
    getline(wordFile, tempString); //Discard first line
    if (wordFile.is_open()){
        while(getline(wordFile,tempString)){
            splitString(tempString, '\t', items);
            temp.setInfo(items);
            items.erase(items.begin(), items.end());
            names.push_back(temp);
        }
    }
}

void fillPrincipalRecords(vector<PrincipalRecord>& records, const string& principalsFile){
    PrincipalRecord temp;
    vector<string> items;

    ifstream wordFile(principalsFile);
    string tempString;
    getline(wordFile, tempString); //Discard first line
    if (wordFile.is_open()){
        while(getline(wordFile,tempString)){
            splitString(tempString, '\t', items);
            temp.setInfo(items);
            items.erase(items.begin(), items.end());
            records.push_back(temp);
        }
    }
}

vector<TitleRecord> findMatchingTitles(vector<TitleRecord> titles, const string& searchWord){
    int low = 0, high = titles.size() - 1, mid = 0;
    TitleRecord temp;
    vector<TitleRecord> foundTitles;
    //Keep finding matching titles until there are no more
    while (mid != -1){
        while (low <= high){
            mid = (high - low) / 2;
            if (searchWord > titles.at(mid).getPrimaryTitle()){
                low = mid + 1;
            }
            else if (searchWord < titles.at(mid).getPrimaryTitle()){
                high = mid - 1;
            }
            else if (searchWord == titles.at(mid).getPrimaryTitle()){
                temp = titles.at(mid);
                foundTitles.push_back(temp);
                titles.erase(titles.begin() + mid);
            }
            else{
                mid = -1;
            }
        }
    }
    return foundTitles;
}

void displayMenu(){
    cout << "Select a menu option:" << endl
         << "\t1. Search for movies" << endl
         << "\t2. Search for actors/actresses" << endl
         << "\t3. Exit" << endl;
    cout << "Your choice --> ";
}

void searchForMovies(const vector<TitleRecord>& movieTitles){
    vector<TitleRecord> foundMovies;
    string search;
    cout << "Enter search phrase: ";
    cin >> search;
    while (int(search.find('+')) != -1) {
        search.at(int(search.find('+'))) = ' ';
    }
    foundMovies = findMatchingTitles(movieTitles, search);
    if ()
}

int run(const string& titlesFile, const string& namesFile, const string& principalsFile) {
    vector<TitleRecord> movieTitles;
    vector<NameRecord> nameRecords;
    vector<PrincipalRecord> principalRecords;

    //fillMovieTitles(movieTitles, titlesFile);
    //fillNameRecords(nameRecords, namesFile);
    //fillPrincipalRecords(principalRecords, principalsFile);

    int menuChoice;
    do{
        displayMenu();
        cin >> menuChoice;
        if (menuChoice == 1){
            searchForMovies(movieTitles);
        }
        break;
    }while(menuChoice != 3);

	return 0;	
}

int main() {
	//Change these addresses according to your local machine
	string titlesFile =	"/Users/krenarbanushi/Documents/CS 141/Project 4/movie.titles.tsv";
	string namesFile = "/Users/krenarbanushi/Documents/CS 141/Project 4/movie.names.tsv";
	string principalsFile = "/Users/krenarbanushi/Documents/CS 141/Project 4/movie.principals.tsv";

	//DO NOT change the main function. The main function will be removed by the autograder
	run(titlesFile, namesFile, principalsFile);
	return 0;
}

