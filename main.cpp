/**********************************************************
 * HEADER
 **********************************************************/

#include <iostream>   
#include <cctype>     
#include <vector>     
#include <sstream>
#include <fstream>
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

void TitleRecord::setInfo(const vector<string>& items) {
    string temp = items.at(8);

    this->titleID = items.at(0);
    this->primaryTitle = items.at(2);
    this->startYear = items.at(5);
    while (int(temp.find(',')) != -1){
        temp.at(temp.find(',')) = ' ';
    }
    this->genres = temp;
}

class NameRecord{
public:
    void setInfo(const vector<string>& items);
    string getNameID() const {return nameID;}
    string getPrimaryName() const {return primaryName;}
    string getBirthYear() const {return birthYear;}
    string getDeathYear() const {return deathYear;}
    string getPrimaryProfession() const {return primaryProfession;}
    vector<string> getKnownTitles() const {return knownTitles;}
private:
    string nameID, primaryName, birthYear, deathYear, primaryProfession;
    vector<string> knownTitles;
};

void NameRecord::setInfo(const vector<string>& items) {
    string temp = items.at(4);
    this->nameID = items.at(0);
    this->primaryName = items.at(1);
    this->birthYear = items.at(2);
    this->deathYear = items.at(3);
    for (int i = 0; i < temp.size(); i++){
        if(temp.at(i) == ','){
            temp.insert(i + 1, " ");
        }
    }
    temp.push_back(',');
    this->primaryProfession = temp;

    temp = items.at(5);
    while (int(temp.find(',')) != -1){
        this->knownTitles.push_back(temp.substr(0, temp.find(',')));
        temp.erase(0, temp.find(',') + 1);
    }
    this->knownTitles.push_back(temp);
}

class PrincipalRecord{
public:
    void setInfo(const vector<string>& items);
    string getTitleID() const {return titleID;}
    string getNameID() const {return nameID;}
    string getCharacters() const {return characters;}
private:
    string titleID = "NONE", nameID = "NONE", characters = "NONE";
}nullRecord;

void PrincipalRecord::setInfo(const vector<string> &items) {
    this->titleID = items.at(0);
    this->nameID = items.at(2);
    this->characters = items.at(5);
}

/*********************************************** Function Prototypes ***************************************/
void searchCharacter(const vector<TitleRecord>& movieTitles, const vector<PrincipalRecord>& principalRecords, const
vector<NameRecord>& names);
void searchForMovies(const vector<TitleRecord>& movieTitles, const vector<PrincipalRecord>& principalRecords, const
vector<NameRecord>& names);
void displayMenu();
void splitString(string line, char delimiter, vector<string>& items);
void fillMovieTitles(vector<TitleRecord>& titles, const string& titlesFile);
void fillNameRecords(vector<NameRecord>& names, const string& namesFile);
void fillPrincipalRecords(vector<PrincipalRecord>& records, const string& principalsFile);
bool containsSearchWord(const TitleRecord& title, const vector<string>& searchWords);
bool containsSearchWord(const NameRecord& title, const vector<string>& searchWords);
string stringToLower(string s);
string getCharacterName(const vector<NameRecord>& names, const string& nameID);
string findMovieName(const vector<TitleRecord>& movieTitles, const string& movieID);
string findCharacterName(const vector<PrincipalRecord>& principalRecords, const string& movieID, const string& characterID);
vector<TitleRecord> findMatchingTitles(const vector<TitleRecord>& titles, const vector<string>& searchWords);
vector<PrincipalRecord> findMovieCharacters(const vector<PrincipalRecord>& records, const string& movieID);
/*********************************************** Helper functions ***************************************/
//The splitString function takes a line and split it on the delimiter and push the parts into items
void splitString(string line, char delimiter, vector<string>& items) {
	stringstream s_stream(line);
	while (s_stream.good()) {
		string item;
		getline(s_stream, item, delimiter);
		items.push_back(item);
	}
}

string stringToLower(string s){
    string temp;
    for (int i = 0; i < s.size(); i++){
        temp += tolower(s.at(i));
    }
    return temp;
}

vector<string> createKeyWords(){
    vector<string> searchWords;
    string search;
    cout << "Enter search phrase: ";
    cin >> search;
    cout << endl;
    splitString(search,'+',searchWords);
    return searchWords;
}

bool containsSearchWord(const TitleRecord& title, const vector<string>& searchWords){
    string tempTitle = stringToLower(title.getPrimaryTitle());
    for (int i = 0; i < searchWords.size(); i++){
        if (int(tempTitle.find(stringToLower(searchWords.at(i)))) < 0){
            return false;
        }
    }
    return true;
}

bool containsSearchWord(const NameRecord& title, const vector<string>& searchWords){
    string tempTitle = stringToLower(title.getPrimaryName());
    for (int i = 0; i < searchWords.size(); i++){
        if (int(tempTitle.find(stringToLower(searchWords.at(i)))) < 0){
            return false;
        }
    }
    return true;
}

vector<TitleRecord> findMatchingTitles(const vector<TitleRecord>& titles, const vector<string>& searchWords){ //FIXME: Should
    // check if the
    vector<TitleRecord> foundTitles;

    for (int i = 0; i < titles.size(); i++){
        if (containsSearchWord(titles.at(i), searchWords)){
            foundTitles.push_back(titles.at(i));
        }
    }

    return foundTitles;
}

vector<PrincipalRecord> findMovieCharacters(const vector<PrincipalRecord>& records, const string& movieID){ //BINARY SEARCH
    vector<PrincipalRecord> movieCharacters;
    for (int i = 0; i < records.size(); i++){
        if (records.at(i).getTitleID() == movieID && records.at(i).getCharacters() != "\\N"){
            movieCharacters.push_back(records.at(i));
        }
    }
    return movieCharacters;
}

string getCharacterName(const vector<NameRecord>& names, const string& nameID){
    int low = 0, mid, high = names.size() - 1;
    while (low <= high){
        mid = (high + low) / 2;
        if (nameID > names.at(mid).getNameID()){
            low = mid + 1;
        }
        else if (nameID < names.at(mid).getNameID()){
            high = mid - 1;
        }
        else if (nameID == names.at(mid).getNameID()){
            return names.at(mid).getPrimaryName();
        }
    }
    return "NULL";
}

vector<NameRecord> findStars(const vector<NameRecord>& names, const vector<string>& keyWords){
    vector<NameRecord> matchingNames;
    for (int i = 0; i < names.size(); i++){
        if (containsSearchWord(names.at(i),keyWords)){
            matchingNames.push_back(names.at(i));
        }
    }
    return matchingNames;
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

void displayMenu(){
    cout << "Select a menu option:" << endl
         << "\t1. Search for movies" << endl
         << "\t2. Search for actors/actresses" << endl
         << "\t3. Exit" << endl;
    cout << "Your choice --> ";
}

void menuOption1(const vector<TitleRecord>& movieTitles, const vector<PrincipalRecord>& principalRecords, const
vector<NameRecord>& names){
    vector<TitleRecord> foundTitles;
    vector<PrincipalRecord> foundCharacters;
    vector<string> searchWords = createKeyWords();

    foundTitles = findMatchingTitles(movieTitles, searchWords);

    for (int i = 0; i < foundTitles.size(); i++){
        cout << i << ":" << endl
             << "Title: " << foundTitles.at(i).getPrimaryTitle() << endl
             << "Year: " << foundTitles.at(i).getStartYear() << endl
             << "Genre: " << foundTitles.at(i).getGenres() << endl
             << "---------------" << endl;
    }

    cout << "Select a movie to see its actors/actresses (-1 to go to the previous menu): ";
    int choice;
    cin >> choice;

    if (choice != -1){
        foundCharacters = findMovieCharacters(principalRecords, foundTitles.at(choice).getTitleID());
        for (int i = 0; i < foundCharacters.size(); i++){
            cout << getCharacterName(names,foundCharacters.at(i).getNameID()) << " " << foundCharacters.at(i).getCharacters() <<
                 endl;
        }
    }
}

string findMovieName(const vector<TitleRecord>& movieTitles, const string& movieID){
    int low = 0, mid, high = movieTitles.size() - 1;
    while (low <= high){
        mid = (low + high) / 2;
        if (movieID > movieTitles.at(mid).getTitleID()){
            low = mid + 1;
        }
        else if (movieID < movieTitles.at(mid).getTitleID()){
            high = mid - 1;
        }
        else if (movieID == movieTitles.at(mid).getTitleID()){
            return movieTitles.at(mid).getPrimaryTitle();
        }
    }
    return "No Movies Found";
}

string findCharacterName(const vector<PrincipalRecord>& principalRecords, const string& movieID, const string& characterID){
    for (int i = 0; i < principalRecords.size(); i++){
        if (principalRecords.at(i).getTitleID() == movieID && principalRecords.at(i).getNameID() == characterID){
            return principalRecords.at(i).getCharacters();
        }
    }
    return "NULL";
}



void menuOption2(const vector<TitleRecord>& movieTitles, const vector<PrincipalRecord>& principalRecords, const
vector<NameRecord>& names){
    vector<string> searchWords = createKeyWords();
    vector<NameRecord> matchingNames = findStars(names,searchWords);
    vector<string> movieIDs;

    cout << "---------------" << endl;
    for (int i = 0; i < matchingNames.size(); i++){
        cout << i << ":" << endl
             << matchingNames.at(i).getPrimaryName() << endl
             << matchingNames.at(i).getPrimaryProfession() << endl
             << "(" << matchingNames.at(i).getBirthYear() << "-" << matchingNames.at(i).getDeathYear() << ")" << endl
             << "---------------" << endl;
    }

    cout << "Select an actor/actress to see movies (-1 to go back to the previous menu): ";
    int choice;
    cin >> choice;
    if (choice != -1){
        cout << endl;

        movieIDs = matchingNames.at(choice).getKnownTitles();

        for (int i = 0; i < movieIDs.size(); i++){
            cout << findMovieName(movieTitles, movieIDs.at(i)) << " ";
            cout << findCharacterName(principalRecords, movieIDs.at(i), matchingNames.at(choice).getNameID()) << endl;
        }
    }

}







int run(const string& titlesFile, const string& namesFile, const string& principalsFile) {
    vector<TitleRecord> movieTitles;
    vector<NameRecord> nameRecords;
    vector<PrincipalRecord> principalRecords;

    fillMovieTitles(movieTitles, titlesFile);
    fillNameRecords(nameRecords, namesFile);
    fillPrincipalRecords(principalRecords, principalsFile);

    int menuChoice;
    do{
        displayMenu();
        cin >> menuChoice;
        switch (menuChoice){
            case 1:
                menuOption1(movieTitles, principalRecords, nameRecords);
                break;
            case 2:
                menuOption2(movieTitles, principalRecords, nameRecords);
                break;
            default:
               exit(0);
        }
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

