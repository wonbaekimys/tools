#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

int main() {
  ifstream fs;
  fs.open("stdout", ifstream::in);
  
  string record;
  double sum = 0;
  while (getline(fs, record)) {
    stringstream ss(record);
    string tmp_str;
    ss >> tmp_str;
    ss >> tmp_str;
    int num;
    ss >> num;
    sum += num / 1000000000.0;
    cout << sum << endl;
  }
  cout << "sum = " << sum << endl;
  return 0;
}
