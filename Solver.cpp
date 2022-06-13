#include <iostream>
#include <fstream>
#include <vector>
//clang++ -std=c++20 Solver.cpp
using namespace std;

//Returns validity of given word
bool isValid(const uint length, const string word, const vector <pair <int, char> > vals, const vector <pair <int, char> > semis, const vector <pair <bool, pair <int, char> > > occs, const vector <char> invs) {
  uint count; //Frequency of given letter
  for (auto val = vals.begin(); val != vals.end(); ++val) {
    if (word[val -> first] != val -> second) {
      return 0;
    }
  }
  for (auto semi = semis.begin(); semi != semis.end(); ++semi) {
    if (word[semi -> first] == semi -> second) {
      return 0;
    }
  }
  for (auto occ = occs.begin(); occ != occs.end(); ++occ) {
    count = 0;
    for (uint i = 0; i < length; ++i) {
      if (word[i] == occ -> second.second) {
        ++count;
      }
    }
    if (occ -> first) {
      if (count < occ -> second.first) {
        return 0;
      }
    } else {
      if (count != occ -> second.first) {
        return 0;
      }
    }
  }
  for (auto inv = invs.begin(); inv != invs.end(); ++inv) {
    for (uint i = 0; i < length; ++i) {
      if (*inv == word[i]) {
        return 0;
      }
    }
  }
  return 1;
}
//Imports words of valid length to new file
void import(const uint length, const string fileName) {
  string word; //Word of word file
  ifstream wFile(fileName); //File of english words by frequency
  ofstream sFile("ValidFile0.txt"); //File of english words of valid length
  while( getline(wFile, word) ) {
    if (word.length() == length) {
        sFile << word << "\n";
    }
  }
  wFile.close();
  sFile.close();
}
//Imports valid words from old file to new file
void sieve(const uint length, uint i, const vector <pair <int, char> > vals, const vector <pair <int, char> > semis, const vector <pair <bool, pair <int, char> > > occs, const vector <char> invs) {
  string word; //Word of word file
  ifstream wFile("ValidFile" + to_string(i) + ".txt"); //Word file
  ofstream sFile("ValidFile" + to_string(i + 1) + ".txt"); //Sieved file
  while( getline(wFile, word) ) {
    if ( isValid(length, word, vals, semis, occs, invs)) {
      sFile << word << "\n";
    }
  }
  wFile.close();
  sFile.close();
}
//Gets the most frequent remaining valid word
string getWord(uint i) {
  string word; //Word of word file
  ifstream wFile("ValidFile" + to_string(i) + ".txt"); //Word file
  getline(wFile, word);
  wFile.close();
  return word;

}
//Repeatedly gets words, until a suitable one is found
string repeatWord(uint i) {
  string word; //Word of word file
  ifstream wFile("ValidFile" + to_string(i) + ".txt"); //Word file
  getline(wFile, word);
  while ( getline(wFile, word) ) {
    cout << "is " << word << " a valid word? Type yes or redo"<< endl;
    cin >> word;
    if (word == "redo") {
      continue;
    } else {
      wFile.close();
      return word;
    }
  }
  return "0";
}
//Takes user input for colors of letters
bool getColors(const uint length, const uint i, bool *irCols, vector <pair <int, char> > *vals, vector <pair <int, char> > *semis, vector <pair <bool, pair <int, char> > > *occs, vector <char> *invs) {
  bool occurType; //Type of occurance
  uint count; //Frequency of given letter
  string cols; //Colors of letters
  string word = getWord(i);
  cout << word << endl;
  cout << "Type in the corrisponding colors; g = green, y = yellow, r = red or grey" << endl;
  cin >> cols;
  if (cols == "done") {
    return 1;
  } else if (cols == "redo") {
    word = repeatWord(i);
    cout << "Type in the corrisponding colors; g = green, y = yellow, r = red or grey" << endl;
    cin >> cols;
  }
  for (uint j = 0; j < length; ++j) {
    for (uint k = j; k < length; ++k) {
      if (word[j] == word[k]) {
        if (cols[j] == 'r' && cols[k] != 'r') {
          semis -> push_back( make_pair(j, word[j]) );
          cols[j] = 'n';
        } else if (cols[j] != 'r' && cols[k] == 'r') {
          semis -> push_back( make_pair(k, word[k]) );
          cols[k] = 'n';
        }
      }
    }
    if (cols[j] == 'g' && !irCols[j]) {
      vals -> push_back( make_pair(j, word[j]) );
      irCols[j] = 1;
    } else if (cols[j] == 'y') {
      semis -> push_back( make_pair(j, word[j]) );
    } else if (cols[j] == 'r') {
      invs -> push_back(word[j]);
    }
  }
  for (uint j = 0; j < length; ++j) {
    if (cols[j] != 'r') {
      occurType = 1;
      count = 0;
      for (uint k = 0; k < length; ++k) {
        if (word[k] == word[j]) {
          if (cols[k] != 'n') {
            ++count;
            cols[k] = 'r';
          } else {
            occurType = 0;
          }
        }
      }
      occs -> push_back( make_pair( occurType, make_pair(count, word[j]) ) );
    }
  }
  return 0;
}

int main() {
  const uint length = 5; //Length of word
  const uint rounds = 6; //Number of rounds
  bool *irCols = (bool*)malloc(length * sizeof(bool));
  vector <pair <int, char> > vals; //Letters known to be at corrisponding indices
  vector <pair <int, char> > semis; //Letters known not to be at corrisponding indices
  vector <pair <bool, pair <int, char> > > occs; //Letters known to have corrisponding frequencies
  vector <char> invs; //Letters known to be in given word
  import(length, "wordFreqs.txt");
  for (uint i = 0; i < rounds; ++i) {
    if (getColors(length, i, irCols, &vals, &semis, &occs, &invs)) {
      break;
    }
    sieve(length, i, vals, semis, occs, invs);
  }
  return 0;
}
