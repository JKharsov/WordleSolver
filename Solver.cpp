#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <tuple>
//clang++ -std=c++20 main.cpp
using namespace std;
void import(const uint len, const string word_list, const string fileName) {
  string word;
  ifstream word_file(word_list);
  ofstream sieved_file(fileName + "0.txt");
  while(getline(word_file, word)) {
    if (word.length() == len) {
      sieved_file << word << endl;
    }
  }
  word_file.close();
  sieved_file.close();
}
uint get_num(const char let, const string word) {
  uint num = 0;
  for (uint i = 0; i < word.length(); ++i) {
    if (word[i] == let) {
      ++num;
    }
  }
  return num;
}
bool is_valid(const string word, const vector <tuple <char, uint, bool> > vals, const vector <tuple <char, uint, bool> > nums) {
  for (auto val = vals.begin(); val != vals.end(); ++val) {
    if (!get<bool>(*val)) {
      if (word[get<uint>(*val)] != get<char>(*val)) {
        return 0;
      }
    } else {
      if (word[get<uint>(*val)] == get<char>(*val)) {
        return 0;
      }
    }
  }
  for (auto num = nums.begin(); num != nums.end(); ++num) {
    if (!get<bool>(*num)) {
      if (get_num(get<char>(*num), word) < get<uint>(*num)) {
        return 0;
      }
    } else {
      if (get_num(get<char>(*num), word) != get<uint>(*num)) {
        return 0;
      }
    }
  }
  return 1;
}
void sieve(const uint i, const string fileName, const vector <tuple <char, uint, bool> > vals, const vector <tuple <char, uint, bool> > nums) {
  string word;
  ifstream word_file(fileName + to_string(i) + ".txt");
  ofstream sieved_file(fileName + to_string(i + 1) + ".txt");
  while(getline(word_file, word)) {
    if (is_valid(word, vals, nums)) {
      sieved_file << word << endl;
    }
  }
  word_file.close();
  sieved_file.close();
}
void clear(const uint i, const uint e, const string fileName) {
  for (uint j = i; j < e + 1; ++j) {
    filesystem::remove(fileName + to_string(j) + ".txt");
  }
}
bool get_numType(const uint i, const uint len, const string word, string &colors, uint &num) {
  bool numType = 0;
  for (uint j = i + 1; j < len; ++j) {
    if (word[i] == word[j]) {
      if (colors[j] == 'g') {
        colors[j] = 'h';
        ++num;
      } else if (colors[j] == 'y') {
        colors[j] = 'z';
        ++num;
      } else {
        colors[j] = 's';
        numType = 1;
      }
    }
  }
  return numType;
}
void set_colors(const uint len, const string word, string colors, bool *irCols, vector <tuple <char, uint, bool> > &vals, vector <tuple <char, uint, bool> > &nums) {
  uint num;
  bool numType;
  for (uint i = 0; i < len; ++i) {
    if (colors[i] == 'g' && !irCols[i]) {
      vals.push_back(make_tuple(word[i], i, 0));
      irCols[i] = 1;
      num = 1;
      numType = get_numType(i, len, word, colors, num);
      if (num != 1) {
        nums.push_back(make_tuple(word[i], num, numType));
      }
    } else if (colors[i] == 'h') {
      vals.push_back(make_tuple(word[i], i, 0));
      irCols[i] = 1;
    } else if (colors[i] == 'y') {
      vals.push_back(make_tuple(word[i], i, 1));
      num = 1;
      numType = get_numType(i, len, word, colors, num);
      nums.push_back(make_tuple(word[i], num, numType));
    } else if (colors[i] == 'z') {
      vals.push_back(make_tuple(word[i], i, 1));
    } else if (colors[i] == 'r') {
      num = 0;
      get_numType(i, len, word, colors, num);
      nums.push_back(make_tuple(word[i], num, 1));
    }
  }
}
bool play_simple(const uint r, const uint len, const string fileName, bool *irCols, vector <tuple <char, uint, bool> > &vals, vector <tuple <char, uint, bool> > &nums) {
  string word, colors;
  ifstream word_file(fileName + to_string(r) + ".txt");
  while(getline(word_file, word)) {
    cout << word << endl;
    cin >> colors;
    if (colors == "redo") {
      continue;
    } else if (colors == "done") {
      word_file.close();
      return 0;
    } else {
      break;
    }
  }
  set_colors(len, word, colors, irCols, vals, nums);
  sieve(r, fileName, vals, nums);
  word_file.close();
  return 1;
}

int main() {
  const uint len = 5;
  const uint rds = 6;
  const string fileName = "sievedFile";
  bool *irCols = (bool*)malloc(len * sizeof(bool));
  bool doRun = 1;
  vector <tuple <char, uint, bool> > vals;
  vector <tuple <char, uint, bool> > nums;
  //import(len, "wordFreqs.txt", fileName);
  while(doRun) {
    for (uint r = 0; r < rds; ++r) {
      if (!play_simple(r, len, fileName, irCols, vals, nums)) {
        doRun = 0;
      }
    }
  }
  clear(1, rds, fileName);
  return 0;
}
