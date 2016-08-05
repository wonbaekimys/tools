#include <chrono>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <ctime>
#include <list>
#include "/home/wb/tools/wbtime.hh"

// #define NUM_CONTAINER 508
#define NUM_CONTAINER 1

using namespace std;

int main() {

  int num_yarn_child = 0;
  double total_yarn_child = 0;
  double total_ugi = 0;
  double total_metrics = 0;
  double total_cred = 0;
  double total_jvmconf = 0;
  double total_logsync = 0;
  double total_jvmtask = 0;
  string prefix = "/home/wb/hadoop/analysis/logs8/userlogs/";
  prefix.append("application_1469456267530_0001/");
  // prefix.append("container_1469456267530_0001_01_");
  ifstream logfile;
  stringstream container_id;
  container_id << setw(6) << setfill('0') << to_string(1);
  string path = prefix;
  path.append("container_1469456267530_0001_01_");
  path.append(container_id.str());
  path.append("/syslog");
  logfile.open(path, ifstream::in);
  if (!logfile.good()) {
    return 0;
  }

  int num_free_slots = 63;
  string record;
  list<WbTime> sche_start;
  map<string, WbTime> start_req;
  map<string, WbTime> start_ass;
  int killing = 0;
  int num_sche = 0;
  int num_launch = 0;
  double total_sche = 0;
  double total_launch = 0;
  bool isis = false;
  while (getline(logfile, record)) {
    if (record.find("Assigned container") != string::npos) {
      size_t found = record.find("container_");
      if (found == string::npos) continue;
      string cid_str = record.substr(found, 38);
      WbTime tmp = WbTime::get_wbtime(record, kHadoop);
      auto it = start_ass.find(cid_str);
      if (it == start_ass.end()) {
        start_ass.emplace(cid_str, tmp);
      }
      if (isis) {
        ++num_sche;
        total_sche += WbTime::get_duration(sche_start.front(), tmp);
        sche_start.pop_front();
      }
    } else if (record.find("Received") != string::npos) {
      isis = true;
      sche_start.emplace_back(WbTime::get_wbtime(record, kHadoop));
    } else if (record.find("REMOTE") != string::npos) {
      size_t found = record.find("container_");
      string cid_str = record.substr(found, 38);
      start_req.emplace(cid_str, WbTime::get_wbtime(record, kHadoop));
    } else {
    }
  }

  int num_req = 0;
  double total_req = 0;
  int num_loc = 0;
  double total_loc = 0;
  int num_entire = 0;
  double total_entire = 0;
  map<string, WbTime> start_loc;
  string nmprefix = "/home/wb/hadoop/analysis/logs8/yarn-wb-nodemanager-raven0";
  for (int i = 1; i <= 8; ++i) {
    string nmpath = nmprefix;
    nmpath.append(to_string(i));
    nmpath.append(".log");
    ifstream nmlog;
    nmlog.open(nmpath, ifstream::in);
    while (getline(nmlog, record)) {
      if (record.find("Start request") != string::npos) {
        size_t found = record.find("container_");
        string cid_str = record.substr(found, 38);
        auto it = start_req.find(cid_str);
        if (it == start_req.end()) {
          continue;
        }
        ++num_req;
        double dr = WbTime::get_duration(it->second,
            WbTime::get_wbtime(record, kHadoop));
        total_req += dr;
      } else if (record.find("to LOCALIZING") != string::npos) {
        size_t found = record.find("container_");
        if (found == string::npos) continue;
        string cid_str = record.substr(found, 38);
        start_loc.emplace(cid_str, WbTime::get_wbtime(record, kHadoop));
      } else if (record.find("to LOCALIZED") != string::npos) {
        size_t found = record.find("container_");
        if (found == string::npos) continue;
        string cid_str = record.substr(found, 38);
        auto it = start_loc.find(cid_str);
        if (it == start_loc.end()) {
          continue;
        }
        ++num_loc;
        double dr = WbTime::get_duration(it->second,
            WbTime::get_wbtime(record, kHadoop));
        total_loc += dr;
      } else if (record.find("bash") != string::npos) {
        auto start_launch = WbTime::get_wbtime(record, kHadoop);
        size_t found = record.find("container_");
        if (found == string::npos) continue;
        string cid_str = record.substr(found, 38);
        string cpath = prefix + cid_str + "/syslog";
        ifstream cfile;
        cfile.open(cpath, ifstream::in);
        if (!cfile.good()) {
          cfile.close();
          continue;
        }
        string tmp_record;
        while (getline(cfile, tmp_record)) {
          if (tmp_record.find("Child starting") != string::npos) {
            ++num_launch;
            WbTime tmp = WbTime::get_wbtime(tmp_record, kHadoop);
            total_launch += WbTime::get_duration(start_launch, tmp);
            auto it = start_ass.find(cid_str);
            if (it != start_ass.end()) {
              ++num_entire;
              double tent = WbTime::get_duration(it->second, tmp);
              total_entire += tent;
            }
            break;
          }
        }
        cfile.close();
      }
    }
    nmlog.close();
  }

  cout << total_sche / num_sche << endl;
  cout << total_req / num_req << endl;
  cout << total_loc / num_loc << endl;
  cout << total_launch / num_launch << endl;
  cout << total_entire / num_entire << endl;

  logfile.close();

  return 0;
}
