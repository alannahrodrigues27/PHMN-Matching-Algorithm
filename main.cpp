#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <fstream> // file input and output
#include <sstream> // parse line
#include <cctype> // toupper and tolower
#include <stdlib.h> // abs()
using namespace std;

// ---------------------------      STRUCTS     ---------------------------

// struct Mentor for information of mentors
struct Mentor {
    string name;
    set <string> highSchool;
    set <string> commMethod;
    set <string> majors;
    set <string> preHealthTrack;
    set <string> universityCategories;
    set <string> extracurriculars;
    set <string> identifyWith;
    set <string> specificSchools;
    set <string> hobbies;
    int grade;
    int timeCommit;
    char communityCollege;
};

// struct Mentee contains information about mentees
struct Mentee {
    string name;
    set <string> highSchool;
    set <string> commMethod;
    set <string> majors;
    set <string> preHealthTrack;
    set <string> universityCategories;
    set <string> specificSchools;
    set <string> extracurriculars;
    set <string> identifyWith;
    set <string> hobbies;
    int grade;
    int timeCommit;
    char communityCollege;
};

// struct to keep track of matches, scores, and reasoning
struct Matches {
    map <string, int> scores;
    map <string, vector<string>> reasons;
};

// -----------------------      UTILITY FUNCTIONS   --------------------------
string trimWhiteSpace(string word) {
    while(word[0] == ' ') {
        word = word.substr(1);
    }

    while(word[word.size() - 1] == ' ') {
        word = word.substr(0, word.size() - 1);
    }

    return word;
}

string toLowerString(string word) {
    for (int i = 0; i < word.size(); i++) {
        word[i] = tolower(word[i]);
    }
    return word;
}

void addToSet(string section, set<string> &newSet) {
    stringstream ss(section);
    string word;

    while(getline(ss, word, ';')) {
        newSet.insert(trimWhiteSpace(toLowerString(word)));
    }
}

void addYear(string word, int &grade) {
    word = toLowerString(word);
    if (word == "senior") { grade = 4; }
    else if (word == "junior") { grade = 3; }
    else if (word == "sophomore") { grade = 2; }
    else if (word == "freshman") { grade = 1; }
    else{ grade = 5; }
}

int findNumMatches(vector <string> &reasonsMatched, const set <string> &items1, const set <string> &items2) {
    int numMatches = 0;
    for (auto item: items1) {
        if (items2.count(item) && item != "-") {
            numMatches++;
            reasonsMatched.push_back(item);
        }
    }
    return numMatches;
}

void addVector(vector <string> &addTo, const vector <string> &addFrom) {
    for (int i = 0; i < addFrom.size(); i ++) {
        addTo.push_back(addFrom.at(i));
    }
}

// ----------------------------        READ IN FILE FUNCTIONS -------------

Matches createMatch (const vector <Mentee>  &mentees) {
    Matches newMatch;
    for (auto mentee : mentees) {
        newMatch.scores[mentee.name] = 0;
        newMatch.reasons[mentee.name] = {};
    }

    return newMatch;
}
Mentor createMentor(const vector <string> &parsedLine) {
    Mentor newMentor;

    //cout << "In new mentor" << endl;

    newMentor.name = parsedLine.at(0);
    newMentor.specificSchools.insert(trimWhiteSpace(toLowerString(parsedLine.at(1))));
    addToSet(parsedLine.at(2),newMentor.majors);
    addYear(parsedLine.at(3), newMentor.grade);
    addToSet(parsedLine.at(4), newMentor.preHealthTrack);
    //cout << newMentor.name << " " << trimWhiteSpace(parsedLine.at(5)) << endl;
    newMentor.timeCommit = stoi(trimWhiteSpace(parsedLine.at(5)));
    addToSet(parsedLine.at(6), newMentor.universityCategories);
    addToSet(parsedLine.at(7), newMentor.specificSchools);
    addToSet(trimWhiteSpace(toLowerString(parsedLine.at(8))), newMentor.highSchool);
    newMentor.communityCollege = trimWhiteSpace(parsedLine.at(9))[0];
    addToSet(parsedLine.at(10), newMentor.hobbies);
    addToSet(parsedLine.at(11), newMentor.extracurriculars);
    addToSet(parsedLine.at(12), newMentor.commMethod);
    addToSet(parsedLine.at(13), newMentor.identifyWith);

    return newMentor;
}

Mentee createMentee(const vector <string> &parsedLine) {
    Mentee newMentee;

    newMentee.name = parsedLine.at(0);
    addToSet(trimWhiteSpace(toLowerString(parsedLine.at(1))), newMentee.highSchool);
    addYear(parsedLine.at(2), newMentee.grade);
    addToSet(parsedLine.at(3), newMentee.majors);
    addToSet(parsedLine.at(4), newMentee.preHealthTrack);
    newMentee.timeCommit = stoi(trimWhiteSpace(parsedLine.at(5)));
    addToSet(parsedLine.at(6), newMentee.universityCategories);
    addToSet(parsedLine.at(7), newMentee.specificSchools);
    newMentee.communityCollege = trimWhiteSpace(parsedLine.at(8))[0];
    addToSet(parsedLine.at(9), newMentee.hobbies);
    addToSet(parsedLine.at(10), newMentee.extracurriculars);
    addToSet(parsedLine.at(11), newMentee.commMethod);
    addToSet(parsedLine.at(12), newMentee.identifyWith);

    return newMentee;
}

void parseLine(string line, vector <string> &parsedLine) {
    stringstream ss(line);
    string word;

    while (getline(ss, word, ',')) {
        if (word.empty()) {
            word = "-";
        }
        parsedLine.push_back(word);
    }
}

bool readInFile(string fileName, vector <string> &lines) {
    ifstream inFS;
    string line;
    inFS.open(fileName);

    if (!inFS.is_open()) {
        cout << fileName << " not open." << endl;
        return false;
    }

    getline(inFS, line);
    while (getline(inFS, line)) {
        if (line.at(0) != ',') {
            lines.push_back(line);
            //cout << line << endl;
        }
    }
    return true;
}

void fillMentors(vector <string> &lines, vector <Mentor> &mentors) {
    vector <string> parsedLine;
    for (auto line: lines) {
        parsedLine.clear();
        parseLine(line, parsedLine);
        if (parsedLine.empty()) {
            continue;
        }
        // << line << endl;
        mentors.push_back(createMentor(parsedLine));
    }
    lines.clear();
   cout << "end of fill mentors" << endl;
}

void fillMentees(vector<string> &lines, vector <Mentee> &mentees) {
    vector <string> parsedLine;
    for (auto line: lines) {
        parsedLine.clear();
        parseLine(line, parsedLine);
        if (parsedLine.empty()) {
            continue;
        }
        mentees.push_back(createMentee(parsedLine));
    }
    lines.clear();
    cout << "end of mentees" << endl;
}

void fillMatches(map <string, Matches> &matches, const vector <Mentor> &mentors, const vector <Mentee> &mentees) {
    for (int i = 0; i < mentors.size(); i++) {
        matches.emplace(mentors.at(i).name, createMatch(mentees));
    }
}

void outputFile(string fileName1, string fileName2, map <string, Matches> &matches, const vector <Mentor> &mentors, const vector <Mentee> &mentees) {
    ofstream outScores;
    ofstream outReasons;
    outScores.open(fileName1);
    outReasons.open(fileName2);

    if (!outScores.is_open()) {
        cout << fileName1 << " not open!" << endl;
        return;
    }
    if (!outReasons.is_open()) {
        cout << fileName2 << " not open!" << endl;
        return;
    }


    outScores << " ,";
    outReasons<< " ,";
    for (int i = 0; i < mentees.size(); i++) {
        outScores << mentees.at(i).name << ",";
        outReasons << mentees.at(i).name << ",";
    }
    outScores << endl;
    outReasons << endl;

    for (int i = 0; i < mentors.size(); i++) {
        outScores << mentors.at(i).name << ",";
        outReasons << mentors.at(i).name << ",";
        for (int j = 0; j < mentees.size(); j++) {
            outScores <<  matches[mentors.at(i).name].scores[mentees.at(j).name] << ",";
            for (int k = 0; k < matches[mentors.at(i).name].reasons[mentees.at(j).name].size(); k++) {
                if (matches[mentors.at(i).name].reasons[mentees.at(j).name].at(k)[0] - 'A' >= 0) {
                    outReasons << matches[mentors.at(i).name].reasons[mentees.at(j).name].at(k)<< ";";
                }
//                if(k == 18) {
//                    cout << mentors.at(i).name << "+" << mentees.at(j).name << ":" << matches[mentors.at(i).name].reasons[mentees.at(j).name].at(k) << endl;
//                }
            }
            outReasons << ",";
        }
        outScores << endl;
        outReasons << endl;
    }

    outScores.close();
    outReasons.close();
}


// ---------------------        FUNCTIONS TO DOUBLE CHECK ----------------
void printSet (const set <string> &items) {
    for (auto item: items) {
        cout << item << ", ";
    }
    cout << endl;
}

void printMentor(const Mentor &mentor) {
    cout << "Name: " << mentor.name << endl;
    cout << "   Majors: ";
    printSet(mentor.majors);
    cout << "   Pre-Health: ";
    printSet(mentor.preHealthTrack);
    cout << "   Specific Schools: ";
    printSet(mentor.specificSchools);
    cout << "\tHigh School: ";
    printSet(mentor.highSchool);
    cout << "\tCommunity College:" << mentor.communityCollege << endl;
    cout << "\tHobbies: ";
    printSet(mentor.hobbies);
    cout << "\tComm Method: ";
    printSet(mentor.commMethod);
    cout << "\tSchool Categories: ";
    printSet(mentor.universityCategories);
    cout << "\tExtracurriculars: ";
    printSet(mentor.extracurriculars);
    cout << "\tIdentities: ";
    printSet(mentor.identifyWith);
    cout << endl << endl;
}

void printMentee(const Mentee &mentee) {
    cout << "Name: " << mentee.name << endl;
    cout << "   Majors: ";
    printSet(mentee.majors);
    cout << "   Pre-Health: ";
    printSet(mentee.preHealthTrack);
    cout << "   Specific Schools: ";
    printSet(mentee.specificSchools);
    cout << "\tHigh School: ";
    printSet(mentee.highSchool);
    cout << "\tCommunity College:" << mentee.communityCollege << endl;
    cout << "\tHobbies: ";
    printSet(mentee.hobbies);
    cout << "\tComm Method: ";
    printSet(mentee.commMethod);
    cout << "\tSchool Categories: ";
    printSet(mentee.universityCategories);
    cout << "\tExtracurriculars: ";
    printSet(mentee.extracurriculars);
    cout << "\tIdentities: ";
    printSet(mentee.identifyWith);
    cout << endl << endl;
}

void printMatch(string name, Matches match) {
    cout << name << endl;
    cout << "SCORES: " << endl;
    for (auto score : match.scores) {
        cout << "\t" << score.first << ": " << score.second << endl;
    }
    cout << "REASONS: " << endl;
    for (auto reasons : match.reasons) {
        cout << "\t" << reasons.first << ": ";
        for (int i = 0; i < reasons.second.size(); i++) {
            cout << reasons.second.at(i) << ", ";
        }
        cout << endl;
    }
}

// ---------------------------      MATCHING FUNCTIONS      ------------------
void comparePreHealth(map <string, Matches> &matches, const vector <Mentor> &mentors, const vector <Mentee> &mentees) {
    int pointsAdded = 5;
    int numMatches = 0;
    vector <string> reasonsMatched;

    for (int i = 0; i < mentors.size(); i++) {
        for (int j = 0; j < mentees.size(); j++) {
            reasonsMatched.clear();
            numMatches = findNumMatches(reasonsMatched, mentors.at(i).preHealthTrack, mentees.at(j).preHealthTrack);
            matches[mentors.at(i).name].scores[mentees.at(j).name] += numMatches * pointsAdded;
            addVector(matches[mentors.at(i).name].reasons[mentees.at(j).name], reasonsMatched);
        }
    }
}

void compareGrade(map <string, Matches> &matches, const vector <Mentor> &mentors, const vector <Mentee> &mentees) {
    int pointsAdded = 2;
    int difference = 0;

    for (int i = 0; i < mentors.size(); i++) {
        for (int j = 0; j < mentees.size(); j++) {
            difference = abs(mentors.at(i).grade - mentees.at(j).grade);
            if (difference < 2) {
                matches[mentors.at(i).name].scores[mentees.at(j).name] += pointsAdded - difference;
                matches[mentors.at(i).name].reasons[mentees.at(j).name].push_back("grade");
            }
        }
    }
}

void compareMajor (map <string, Matches> &matches, const vector <Mentor> &mentors, const vector <Mentee> &mentees) {
    int pointsAdded = 4;
    int numMatches = 0;
    vector <string> reasonsMatched;

    for (int i = 0; i < mentors.size(); i++) {
        for (int j = 0; j < mentees.size(); j++) {
            reasonsMatched.clear();
            numMatches = findNumMatches(reasonsMatched, mentors.at(i).majors, mentees.at(j).majors);
            matches[mentors.at(i).name].scores[mentees.at(j).name] += numMatches * pointsAdded;
            addVector(matches[mentors.at(i).name].reasons[mentees.at(j).name], reasonsMatched);
        }
    }
}

void compareHighSchool(map <string, Matches> &matches, const vector <Mentor> &mentors, const vector <Mentee> &mentees) {
    int pointsAdded = 3;
    int numMatches = 0;
    vector <string> reasonsMatched;

    for (int i = 0; i < mentors.size(); i++) {
        for (int j = 0; j <mentees.size(); j++) {
            reasonsMatched.clear();
            numMatches = findNumMatches(reasonsMatched, mentors.at(i).highSchool, mentees.at(j).highSchool);
            matches[mentors.at(i).name].scores[mentees.at(j).name] += numMatches * pointsAdded;
            addVector(matches[mentors.at(i).name].reasons[mentees.at(j).name], reasonsMatched);
        }
    }
}

void compareUniversities(map <string, Matches> &matches, const vector <Mentor> &mentors, const vector <Mentee> &mentees) {
    int pointsAdded = 3;
    int numMatches = 0;
    vector <string> reasonsMatched;

    for (int i = 0; i < mentors.size(); i++) {
        for (int j = 0; j < mentees.size(); j++) {
            reasonsMatched.clear();
            numMatches = findNumMatches(reasonsMatched, mentors.at(i).specificSchools, mentees.at(j).specificSchools);
            matches[mentors.at(i).name].scores[mentees.at(j).name] += numMatches * pointsAdded;
            addVector(matches[mentors.at(i).name].reasons[mentees.at(j).name], reasonsMatched);
        }
    }
}

void compareUniversityCategories(map <string, Matches> &matches, const vector <Mentor> &mentors, const vector <Mentee> &mentees) {
    int pointsAdded = 2;
    int numMatches = 0;
    vector <string> reasonsMatched;

    for (int i = 0; i < mentors.size(); i++) {
        for (int j = 0; j < mentees.size(); j++) {
            reasonsMatched.clear();
            numMatches = findNumMatches(reasonsMatched, mentors.at(i).universityCategories, mentees.at(j).universityCategories);
            matches[mentors.at(i).name].scores[mentees.at(j).name] += numMatches * pointsAdded;
            addVector(matches[mentors.at(i).name].reasons[mentees.at(j).name], reasonsMatched);
        }
    }
}

void compareExtracurriculars(map <string, Matches> &matches, const vector <Mentor> &mentors, const vector <Mentee> &mentees) {
    int pointsAdded = 1;
    int numMatches = 0;
    vector <string> reasonsMatched;

    for (int i = 0; i < mentors.size(); i++) {
        for (int j = 0; j < mentees.size(); j++) {
            reasonsMatched.clear();
            numMatches = findNumMatches(reasonsMatched, mentors.at(i).extracurriculars, mentees.at(j).extracurriculars);
            matches[mentors.at(i).name].scores[mentees.at(j).name] += numMatches * pointsAdded;
            addVector(matches[mentors.at(i).name].reasons[mentees.at(j).name], reasonsMatched);
        }
    }
}

void compareHobbies(map <string, Matches> &matches, const vector <Mentor> &mentors, const vector <Mentee> &mentees) {
    int pointsAdded = 1;
    int numMatches = 0;
    vector <string> reasonsMatched;

    for (int i = 0; i < mentors.size(); i++) {
        for (int j = 0; j < mentees.size(); j++) {
            reasonsMatched.clear();
            numMatches = findNumMatches(reasonsMatched, mentors.at(i).hobbies, mentees.at(j).hobbies);
            matches[mentors.at(i).name].scores[mentees.at(j).name] += numMatches * pointsAdded;
            addVector(matches[mentors.at(i).name].reasons[mentees.at(j).name], reasonsMatched);
        }
    }
}

void compareComm(map <string, Matches> &matches, const vector <Mentor> &mentors, const vector <Mentee> &mentees) {
    int pointsAdded = 1;
    int numMatches = 0;
    vector <string> reasonsMatched;

    for (int i = 0; i < mentors.size(); i++) {
        for (int j = 0; j < mentees.size(); j++) {
            reasonsMatched.clear();
            numMatches = findNumMatches(reasonsMatched, mentors.at(i).commMethod, mentees.at(j).commMethod);
            matches[mentors.at(i).name].scores[mentees.at(j).name] += numMatches * pointsAdded;
            addVector(matches[mentors.at(i).name].reasons[mentees.at(j).name], reasonsMatched);
        }
    }
}

void compareCommunityCollege(map <string, Matches> &matches, const vector <Mentor> &mentors, const vector <Mentee> &mentees) {
    int pointsAdded = 3;

    for (int i = 0; i < mentors.size(); i++) {
        for (int j = 0; j <mentees.size(); j++) {
            if (mentors.at(i).communityCollege== 'Y' && (mentors.at(i).communityCollege == mentees.at(j).communityCollege)) {
                matches[mentors.at(i).name].scores[mentees.at(j).name] += pointsAdded;
                matches[mentors.at(i).name].reasons[mentees.at(j).name].push_back("community college");
            }
        }
    }
}

void compareIdentities(map <string, Matches> &matches, const vector <Mentor> &mentors, const vector <Mentee> &mentees) {
    int pointsAdded = 2;
    int numMatches = 0;
    vector <string> reasonsMatched;

    for (int i = 0; i < mentors.size(); i++) {
        for (int j = 0; j < mentees.size(); j++) {
            //cout << "mentor: " << mentors.at(i).name << "  mentee:" << mentees.at(j).name << endl;
            reasonsMatched.clear();
            numMatches = findNumMatches(reasonsMatched, mentors.at(i).identifyWith, mentees.at(j).identifyWith);
            if (reasonsMatched.size() != 0 && reasonsMatched.at(0) == "x") {
                continue;
            }
            matches[mentors.at(i).name].scores[mentees.at(j).name] += numMatches * pointsAdded;
            addVector(matches[mentors.at(i).name].reasons[mentees.at(j).name], reasonsMatched);
        }
    }
}




// -----------------------------        MAIN        ----------------------------------
int main() {
    vector <Mentor> mentors;
    vector <Mentee> mentees;
    map <string, Matches> matches;
    vector <string> lines;
    readInFile("mentorsSpring2024.csv", lines); // Update filename
    fillMentors(lines, mentors);
    readInFile("menteesSpring2024.csv", lines); // Update filename
    fillMentees(lines, mentees);
    fillMatches(matches, mentors, mentees);

    cout << "pre health" << endl;
    comparePreHealth(matches, mentors, mentees);
    compareGrade(matches, mentors, mentees);
    cout << "major" << endl;
    compareMajor(matches, mentors, mentees);
    cout << "high school" << endl;
    compareHighSchool(matches, mentors, mentees);
    cout << "universities" << endl;
    compareUniversities(matches, mentors, mentees);
    cout << "university categories" << endl;
    compareUniversityCategories(matches, mentors, mentees);
    cout << "extra" << endl;
    compareExtracurriculars(matches, mentors, mentees);
    cout << "hobbies" << endl;
    compareHobbies(matches, mentors, mentees);
    compareComm(matches, mentors, mentees);
    compareCommunityCollege(matches, mentors, mentees);
    cout << "identities" << endl;
    compareIdentities(matches, mentors, mentees);

    cout << "matches" <<  endl;
    cout << matches.size() << endl;
    //for (auto match: matches) {
    //    printMatch(match.first, match.second);
    //}

    outputFile("scores24Spring.csv", "reasons24Fall.csv", matches, mentors, mentees); // Update filenames

    return 0;
}
