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

//This function will fill all of the data members of the TitleRecord object using a vector.
//This function takes in a string vector and has a void return type
void TitleRecord::setInfo(const vector<string>& items) {
    string temp = items.at(8); // Genres
    this->titleID = items.at(0); //TitleID
    this->primaryTitle = items.at(2); //PrimaryTitle
    this->startYear = items.at(5); //StartYear
    while (int(temp.find(',')) != -1){ //Replace all commas with spaces in the genres
        temp.at(temp.find(',')) = ' ';
    }
    this->genres = temp; //Assign the genres with the modified version with spaces instead of commas
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
    string nameID, primaryName, birthYear, deathYear, primaryProfession;
};

//This function will fill all of the data members of the NameRecord object using a vector.
//This function takes in a string vector and has a void return type
void NameRecord::setInfo(const vector<string>& items) {
    string temp = items.at(4);
    this->nameID = items.at(0); //NameID
    this->primaryName = items.at(1);
    this->birthYear = items.at(2);
    this->deathYear = items.at(3);
    for (int i = 0; i < temp.size(); i++){ //This will add a space after every comma in the primaryProfessions
        if(temp.at(i) == ','){
            temp.insert(i + 1, " ");
        }
    }
    temp.push_back(','); //Add a final comma (To replicate the examples)
    this->primaryProfession = temp; //Assign the promaryProfession string to the modified version with spaces and an additional ,
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

//This function will fill all of the data members of the PrincipalRecord object using a vector.
//This function takes in a string vector and has a void return type
void PrincipalRecord::setInfo(const vector<string> &items) {
    this->titleID = items.at(0);
    this->nameID = items.at(2);
    this->characters = items.at(5);
}

/*********************************************** Function Prototypes ***************************************/
void displayMovieAndCharacter(const vector<PrincipalRecord>& records, const vector<TitleRecord>& titles, const string&characterID);
void displayMenu();
void splitString(string line, char delimiter, vector<string>& items);
void fillMovieTitles(vector<TitleRecord>& titles, const string& titlesFile);
void fillNameRecords(vector<NameRecord>& names, const string& namesFile);
void fillPrincipalRecords(vector<PrincipalRecord>& records, const string& principalsFile);
bool containsSearchWord(const TitleRecord& title, const vector<string>& searchWords);
bool containsSearchWord(const NameRecord& name, const vector<string>& searchWords);
string stringToLower(string s);
string getCharacterName(const vector<NameRecord>& names, const string& nameID);
string findMovieName(const vector<TitleRecord>& movieTitles, const string& movieID);
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

//StringToLower will take in a string as a argument and return the string in completely lowercase
string stringToLower(string s){
    string temp;
    for (int i = 0; i < s.size(); i++){
        temp += tolower(s.at(i));
    }
    return temp; //Return lowercase string
}

//Create keywords will create a string vector with each element being a word the user typed in
//This function takes in no arguments and returns a string type vector
vector<string> createKeyWords(){
    vector<string> searchWords;
    string search;
    cout << "Enter search phrase: ";
    cin >> search;
    splitString(search,'+',searchWords);
    return searchWords; //Return a vector of the words the user entered
}

//This function will  check if there is a movie title contains the search criteria the user provided
//This function takes in a TitleRecord and string vector.  It returns true or false.
bool containsSearchWord(const TitleRecord& title, const vector<string>& searchWords){
    string tempTitle = stringToLower(title.getPrimaryTitle());
    for (int i = 0; i < searchWords.size(); i++){
        if (int(tempTitle.find(stringToLower(searchWords.at(i)))) < 0){
            return false;
        }
    }
    return true; //Returns true if the title contained all the search words
}

//This function will check if there is a actor/actress name that contains the search criteria the user provided
//This function takes in a NameRecord and string vector.  It returns true or false
bool containsSearchWord(const NameRecord& name, const vector<string>& searchWords){
    string tempTitle = stringToLower(name.getPrimaryName());
    for (int i = 0; i < searchWords.size(); i++){
        if (int(tempTitle.find(stringToLower(searchWords.at(i)))) < 0){
            return false;
        }
    }
    return true; //Returns true if the name contains all the search words
}

//This function will check every element in the TitleRecords vector to find matches with the user's search criteria.
//This function takes in a TitleRecord vector and a string vector.  It returns all the TitleRecords that matched in a vector.
vector<TitleRecord> findMatchingTitles(const vector<TitleRecord>& titles, const vector<string>& searchWords){
    vector<TitleRecord> foundTitles;
    for (int i = 0; i < titles.size(); i++){
        if (containsSearchWord(titles.at(i), searchWords)){
            foundTitles.push_back(titles.at(i));
        }
    }
    return foundTitles; //Return the found titles matching the search criteria
}

//This function will find the characters that appeared in a movie by searching the principal records with the movieID.
//The function will take in the PrincipalRecords vector and the movieID as a string.
//The function will return all the movie characters in a principalRecord vector.
vector<PrincipalRecord> findMovieCharacters(const vector<PrincipalRecord>& records, const string& movieID){
    vector<PrincipalRecord> movieCharacters;
    for (int i = 0; i < records.size(); i++){
        if (records.at(i).getTitleID() == movieID && records.at(i).getCharacters() != "\\N"){
            movieCharacters.push_back(records.at(i));
        }
    }
    return movieCharacters; //Return PrincipalRecord vector =
}

//This function will find the actor/actress name from the NameRecords vector using their nameID
//This function takes in a vector of nameRecords and a string of the actor/actress's nameID
//This function will return the name of the character as a string.
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
            return names.at(mid).getPrimaryName(); //Return the character name
        }
    }
    return "NULL"; //This should never be used since we are searching using the person's ID.
}

//This function will find the name of an actor/actress based on the search criteria provided
//This function takes in a vector of NameRecord and a vector of strings
//This function returns the names that matched in a vector of NameRecord
vector<NameRecord> findActorsActresses(const vector<NameRecord>& names, const vector<string>& keyWords){
    vector<NameRecord> matchingNames;
    for (int i = 0; i < names.size(); i++){
        if (containsSearchWord(names.at(i),keyWords)){
            matchingNames.push_back(names.at(i));
        }
    }
    return matchingNames; //Returns all the names that fit the criteria
}

//This function will find the name of a movie based on the movieID.
//This function takes in a vector of titleRecord and a string of the movie ID.
//This function returns the name of the movie as a string
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
            return movieTitles.at(mid).getPrimaryTitle() + " " + movieTitles.at(mid).getStartYear();
        }
    }
    return "No Movies Found"; //This should not be used since we are searching using the movieID
}

//This function is used to display all of the movies a person has appeared in plus their character name.
//This function takes in a vector of PrincipalRecord, a vector of TitleRecord, and a string of the person's ID.
//This function has a void return type, but it does output the information in the console.
void displayMovieAndCharacter(const vector<PrincipalRecord>& records, const vector<TitleRecord>& titles, const string&
characterID){
    for (int i = 0; i < records.size(); i++){
        if (records.at(i).getNameID() == characterID){
            cout << findMovieName(titles, records.at(i).getTitleID()) << " " << records.at(i).getCharacters() << endl;
        }
    }
}
/**********************************************************************************************************/
//This function is used to fill the vector of TitleRecords with the information from the provided files.
//This function takes in a TitleRecord vector by reference and the location of the file in a string.
//This function has a void return type.
void fillMovieTitles(vector<TitleRecord>& titles, const string& titlesFile){
    TitleRecord temp;
    vector<string> items;

    ifstream wordFile(titlesFile); //Open file
    string tempString; //Used to read and append line to vector
    getline(wordFile,tempString); //First line is not necessary
    if(wordFile.is_open()){ //Check if the file has been opened first
        while(getline(wordFile,tempString)) {  //While there are still lines to read
            splitString(tempString, '\t', items); //Split up the items and assign them to a temporary vector.
            temp.setInfo(items); //Fill in the object information using the vector
            items.erase(items.begin(), items.end()); //Empty the vector
            titles.push_back(temp);
        }
    }
    wordFile.close(); //Close file
}

//This function is used to fill the vector of NameRecord with the information from the provided files.
//This function takes in a NameRecord vector by reference and the location of the file in a string.
//This function has a void return type.
void fillNameRecords(vector<NameRecord>& names, const string& namesFile){
    NameRecord temp;
    vector<string> items;

    ifstream wordFile(namesFile);
    string tempString;
    getline(wordFile, tempString); //Discard first line
    if (wordFile.is_open()){
        while(getline(wordFile,tempString)){
            splitString(tempString, '\t', items); //Split up line and assign to string vector
            temp.setInfo(items); //Fill in object variables using string vector
            items.erase(items.begin(), items.end());
            names.push_back(temp); //Add initialized object to vector
        }
    }
    wordFile.close(); //Close file
}

//This function is used to fill the vector of PrincipalRecord with the information from the provided files.
//This function takes in a PrincipalRecord vector by reference and the location of the file in a string.
//This function has a void return type.
void fillPrincipalRecords(vector<PrincipalRecord>& records, const string& principalsFile){
    PrincipalRecord temp;
    vector<string> items;

    ifstream wordFile(principalsFile);
    string tempString;
    getline(wordFile, tempString); //Discard first line
    if (wordFile.is_open()){
        while(getline(wordFile,tempString)){
            splitString(tempString, '\t', items); //Split up line and assign to string vector
            temp.setInfo(items); //Fill in object variables using string vector
            items.erase(items.begin(), items.end());
            records.push_back(temp); //Add object to vector of PrincipalRecord
        }
    }
    wordFile.close(); //Close file
}

//This function is used to display the main menu of the program.
//This function takes no arguments and has a void return type.
void displayMenu(){
    cout << "Select a menu option:" << endl
         << "\t1. Search for movies" << endl
         << "\t2. Search for actors/actresses" << endl
         << "\t3. Exit" << endl;
    cout << "Your choice --> ";
}

//This function is used to complete the first menu option (Search for movies)
//This function takes in a vector of TitleRecord, vector of PrincipalRecord, and a vector of NameRecord
//This function has a void return type
void menuOption1(const vector<TitleRecord>& movieTitles, const vector<PrincipalRecord>& principalRecords, const
vector<NameRecord>& names){
    vector<TitleRecord> foundTitles;
    vector<PrincipalRecord> foundCharacters;
    vector<string> searchWords = createKeyWords();

    //Collect all of the matching titles in a vector
    foundTitles = findMatchingTitles(movieTitles, searchWords);
    if (!foundTitles.empty()) { //Verify there were matches to the user's search
        cout << endl;
        for (int i = 0; i < foundTitles.size(); i++) {
            cout << i << ":" << endl
                 << "Title: " << foundTitles.at(i).getPrimaryTitle() << endl
                 << "Year: " << foundTitles.at(i).getStartYear() << endl
                 << "Genre: " << foundTitles.at(i).getGenres() << endl
                 << "---------------" << endl;
        }
        cout << "Select a movie to see its actors/actresses (-1 to go to the previous menu): ";
        int choice;
        cin >> choice;

        if (choice != -1 && !cin.fail()) { //Verify the user wants to select a movie
            foundCharacters = findMovieCharacters(principalRecords, foundTitles.at(choice).getTitleID());
            for (int i = 0; i < foundCharacters.size(); i++) {
                cout << getCharacterName(names, foundCharacters.at(i).getNameID()) << " " << foundCharacters.at(i).getCharacters() << endl;
            }
        }
    }
    else{
        cout << "No match found!" << endl; //If there were no matching titles found
    }
}

//This function is used to complete the second menu option (Search for actors/actresses)
//This function takes in a vector of TitleRecord, vector of PrincipalRecord, and vector of NameRecord
//This function has a void return type.
void menuOption2(const vector<TitleRecord>& movieTitles, const vector<PrincipalRecord>& principalRecords, const
vector<NameRecord>& names){
    vector<string> searchWords = createKeyWords();
    vector<NameRecord> matchingNames = findActorsActresses(names,searchWords);
    vector<string> movieIDs;

    if (!matchingNames.empty()) { //Verify there were names that matched the criteria
        cout << "---------------" << endl;
        for (int i = 0; i < matchingNames.size(); i++) {
            cout << i << ":" << endl
                 << matchingNames.at(i).getPrimaryName() << endl
                 << matchingNames.at(i).getPrimaryProfession() << endl
                 << "(" << matchingNames.at(i).getBirthYear() << "-" << matchingNames.at(i).getDeathYear() << ")" << endl
                 << "---------------" << endl;
        }

        cout << "Select an actor/actress to see movies (-1 to go back to the previous menu): ";
        int choice;
        cin >> choice;
        if (choice != -1 && !cin.fail()) { //Verify the user made a valid choice
            displayMovieAndCharacter(principalRecords, movieTitles, matchingNames.at(choice).getNameID());
        }
    }
    else{
        cout << "No match found!" << endl;
    }
}

//This function serves as the program's main function.  This function displays the main menu and will allow for the user to
// choose which function to run until they decide to end the program.
//This function takes in 3 strings signifying the file locations of the movieTitles, characterNames, and principalFile
//This function has a return type of integer.
int run(const string& titlesFile, const string& namesFile, const string& principalsFile) {
    vector<TitleRecord> movieTitles;
    vector<NameRecord> nameRecords;
    vector<PrincipalRecord> principalRecords;

    fillMovieTitles(movieTitles, titlesFile);
    fillNameRecords(nameRecords, namesFile);
    fillPrincipalRecords(principalRecords, principalsFile);

    int menuChoice;
    displayMenu();
    cin >> menuChoice;
    while (!cin.fail() && menuChoice != 3){ //Verify the user made a valid choice and/or doesn't want to exit the program.
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
        displayMenu();
        cin >> menuChoice;
    }
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

