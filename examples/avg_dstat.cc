#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// [job_name]-[data_size]-[block_size]-{run_id}-{host}.csv

int main(int argc, char *argv[]) {
  if (argc < 4) {
    cout << "usage: avg_dstat job_name data_size block_size [num_runs] [num_slaves]" << endl;
    return 1;
  }
  const string kdstat_dir = "/home/wb/hadoop/eval/dstat";
  const string kjob_name(argv[1]);
  const string kdata_size(argv[2]);
  const string kblock_size(argv[3]);
  const int knum_runs = (argc > 4)?atoi(argv[4]):1;
  const int knum_slaves = (argc > 5)?atoi(argv[5]):32;

  // vector<vector<ifstream>> ifs(knum_runs, vector<ifstream>(knum_slaves));
  vector<vector<ifstream>> ifs;
  string prefix = kdstat_dir;
  prefix.append("/");
  prefix.append(kjob_name);
  prefix.append("-");
  prefix.append(kdata_size);
  prefix.append("-");
  prefix.append(kblock_size);

  vector<string> heads(8);
  ifs.resize(knum_runs);
  for (int i = 0; i < knum_runs; ++i) {
    ifs[i].resize(knum_slaves);
    for (int j = 0; j < knum_slaves; ++j) {
      stringstream ss;
      ss << setw(2) << setfill('0');
      ss << j + 1;
      string path = prefix;
      path.append("-");
      path.append(to_string(i + 1));
      path.append("-");
      path.append("raven");
      path.append(ss.str());
      path.append(".csv");
      cout << path << endl;
      ifs[i][j].open(path, ifstream::in);
      for (int k = 0; k < 8; ++k) {
        getline(ifs[i][j], heads[k]);
      }
    }
  }

  ofstream ofs;
  string output_path = prefix;
  output_path.append("-avg.csv");
  cout << output_path << endl;
  ofs.open(output_path, ofstream::out);
  for (int i = 0; i < 2; ++i) {
    cout << heads[i + 5] << endl;
    ofs << heads[i + 5] << endl;
  }
  bool flag = true;
  while (flag) {
    vector<double> attrs(18, 0);
    for (int i = 0; i < knum_runs; ++i) {
      for (int j = 0; j < knum_slaves; ++j) {
        string record;
        if (!getline(ifs[i][j], record)) {
          i = knum_runs;
          flag = false;
          break;
        }
        istringstream iss(record);
        string token;
        int att_cnt = 0;
        while (getline(iss, token, ',')) {
          attrs[att_cnt] += stod(token);
          ++att_cnt;
        }
      }
    }
    if (flag) {
      for (int i = 0; i < 18; ++i) {
        attrs[i] /= (knum_runs * knum_slaves);
        ofs << attrs[i] << ",";
      }
      ofs << endl;
    }
  }
  ofs.close();
  for (int i = 0; i < knum_runs; ++i) {
    for (int j = 0; j < knum_slaves; ++j) {
      ifs[i][j].close();
    }
  }
  return 0;
}
