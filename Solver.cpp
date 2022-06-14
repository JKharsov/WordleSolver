#include <iostream>
#include <fstream>
#include <vector>
//clang++ -std=c++20 Solver.cpp
using namespace std;

string getCols(const uint i, string &word) {
  string cols;
  ifstream wFile ("SievedFile" + std::to_string(i) + ".txt");
  while (getline (wFile, word) ) {
    cout << "The word is " << word << ". Please type in the colors; g = green, y = yellow, r = grey" << endl;
    cin >> cols;
    if (cols == "done") {
      wFile.close();
      return "done";
    } else if (cols == "redo") {
      continue;
    } else {
      break;
    }
  }
  wFile.close();
  return cols;
}
uint getCount(const char let, const string word) {
  uint count = 0;
  for (uint i = 0; i < word.length(); ++i) {
    if (word[i] == let) {
      ++count;
    }
  }
  return count;
}
bool isValid(const uint len, const string word, vector <pair <char, int> > vals, vector <pair <char, int> > semis, vector <pair <char, int> > exts, vector <pair <char, int> > mins, vector <char> invs) {
  for (auto val = vals.begin(); val != vals.end(); ++val) {
    if (word[val -> second] != val -> first) {
      return 0;
    }
  }
  for (auto semi = semis.begin(); semi != semis.end(); ++semi) {
    if (word[semi -> second] == semi -> first) {
      return 0;
    }
  }
  for (auto ext = exts.begin(); ext != exts.end(); ++ext) {
    if (getCount(ext -> first, word) < ext -> second) {
      return 0;
    }
  }
  for (auto min = mins.begin(); min != mins.end(); ++min) {
    if (getCount(min -> first, word) < min -> second) {
      return 0;
    }
  }
  for (auto inv = invs.begin(); inv != invs.end(); ++inv) {
    for (uint i = 0; i < len; ++i) {
      if (*inv == word[i]) {
        return 0;
      }
    }
  }
  return 1;
}
void import(const uint len, const string wList) {
  string word;
  ifstream wFile (wList);
  ofstream sFile ("SievedFile0.txt");
  while(getline(wFile, word)) {
    if (word.length() == len) {
      sFile << word << endl;
    }
  }
  wFile.close();
  sFile.close();
}
void sieve(const uint len, uint i, vector <pair <char, int> > vals, vector <pair <char, int> > semis, vector <pair <char, int> > exts, vector <pair <char, int> > mins, vector <char> invs) {
  string word;
  ifstream wFile ("SievedFile" + std::to_string(i) + ".txt");
  ofstream sFile ("SievedFile" + std::to_string(i + 1) + ".txt");
  while(getline(wFile, word)) {
    if (isValid(len, word, vals, semis, exts, mins, invs)) {
      sFile << word << "\n";
    }
  }
  wFile.close();
  sFile.close();
}
void getColor(const uint len, string cols, string word, bool *irCols, vector <pair <char, int> > *vals, vector <pair <char, int> > *semis, vector <pair <char, int> > *exts, vector <pair <char, int> > *mins, vector <char> *invs) {
  uint count, oType;
  for (uint i = 0; i < len; ++i) {
    for (uint j = i; j < len; ++j) {
      if (word[i] == word[j]) {
        if (cols[i] == 'r' && cols[j] != 'r') {
          semis -> push_back(make_pair(word[i], i));
          cols[i] = 'n';
        } else if (cols[i] != 'r' && cols[j] == 'r') {
          semis -> push_back(make_pair(word[j], j));
          cols[j] = 'n';
        }
      }
    }
    if (irCols[i]) {
      continue;
    } else if (cols[i] == 'g') {
      vals -> push_back(make_pair(word[i], i));
      irCols[i] = 1;
    } else if (cols[i] == 'y') {
      semis -> push_back(make_pair(word[i], i));
    } else if (cols[i] == 'r') {
      invs -> push_back(word[i]);
    }
  }
  for (uint i = 0; i < len; ++i) {
    if (cols[i] == 'y') {
      count = 1;
      oType = 0;
      for (uint j = i + 1; j < len; ++j) {
        if (word[i] == word[j]) {
          if (cols[j] == 'g' || cols[j] == 'y') {
            ++count;
          } else {
            oType = 1;
          }
          cols[j] = 'r';
        }
      }
      if (oType) {
        exts -> push_back( make_pair(word[i], count) );
      } else {
        mins -> push_back( make_pair(word[i], count) );
      }
    }
    if (cols[i] ==  'n') {
      count = 0;
      for (uint j = i + 1; j < len; ++j) {
        if (word[i] == word[j]) {
          if (cols[j] == 'g' || cols[j] == 'y') {
            ++count;
          }
          cols[j] = 'r';
        }
      }
      exts -> push_back( make_pair(word[i], count) );
    }
  }

}

int main() {
  const uint len = 5;
  const uint rounds = 6;
  string word, cols;
  bool *irCols = (bool*)malloc(len * sizeof(bool));
  vector <pair <char, int> > vals;
  vector <pair <char, int> > semis;
  vector <pair <char, int> > exts;
  vector <pair <char, int> > mins;
  vector <char> invs;
  import(len, "wordList.txt");
  for (uint i = 0; i < rounds; ++i) {
    cols = getCols(i, word);
    if (cols == "done") {
      break;
    } else {
      getColor(len, cols, word, irCols, &vals, &semis, &exts, &mins, &invs);
      sieve(len, i, vals, semis, exts, mins, invs);
    }
  }
  return 0;
}
