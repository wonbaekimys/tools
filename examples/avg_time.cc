#include <chrono>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <ctime>
#include "/home/wb/tools/wbtime.hh"

#define NUM_CONTAINER 508
// #define NUM_CONTAINER 3

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
  prefix.append("container_1469456267530_0001_01_");
  for (int i = 2; i <= NUM_CONTAINER; ++i) {
    ifstream logfile;
    stringstream container_id;
    container_id << setw(6) << setfill('0') << to_string(i);
    string path = prefix + container_id.str() + "/syslog";

    logfile.open(path, ifstream::in);

    if (!logfile.good()) {
      continue;
    }

    string record;
    WbTime yarn_start_t;
    WbTime ugi_start_t;
    WbTime metrics_start_t;
    WbTime cred_start_t;
    WbTime umb_start_t;
    WbTime jvmconf_start_t;
    WbTime jvminit_start_t;
    WbTime logsync_start_t;
    WbTime jvmtask_start_t;
    WbTime map_start_t;
    bool is_yarn_child = false;
    bool is_task_run = false;
    bool is_conf_task = false;
    while (getline(logfile, record)) {
      if (!is_yarn_child && record.find("Child starting") != string::npos) {
        is_yarn_child = true;
        yarn_start_t = WbTime::get_wbtime(record, kHadoop);
      } else if (record.find("UserGroupInformation.set") != string::npos) {
        ugi_start_t = WbTime::get_wbtime(record, kHadoop);
      } else if (record.find("Metric initializing") != string::npos) {
        metrics_start_t = WbTime::get_wbtime(record, kHadoop);
      } else if (record.find("getCurrentUser()") != string::npos) {
        cred_start_t = WbTime::get_wbtime(record, kHadoop);
      } else if (record.find("RPC.getProxy()") != string::npos) {
        umb_start_t = WbTime::get_wbtime(record, kHadoop);
      } else if (!is_conf_task && record.find("configureT") != string::npos) {
        is_conf_task = true;
        jvmconf_start_t = WbTime::get_wbtime(record, kHadoop);
      } else if (record.find("initSi") != string::npos) {
        jvminit_start_t = WbTime::get_wbtime(record, kHadoop);
      } else if (record.find("LogS") != string::npos) {
        logsync_start_t = WbTime::get_wbtime(record, kHadoop);
      } else if (record.find("PrivilegedExceptionAction.run(): Task.run()")
          != string::npos) {
        jvmtask_start_t = WbTime::get_wbtime(record, kHadoop);
      } else if (!is_task_run && record.find("MapTask: P") != string::npos) {
        is_task_run = true;
        map_start_t = WbTime::get_wbtime(record, kHadoop);
      }
    }
    if (is_yarn_child && is_task_run) {
      ++num_yarn_child;
      total_yarn_child += WbTime::get_duration(yarn_start_t, map_start_t);
      total_ugi += WbTime::get_duration(ugi_start_t, metrics_start_t);
      total_metrics += WbTime::get_duration(metrics_start_t, cred_start_t);
      total_cred += WbTime::get_duration(cred_start_t, umb_start_t);
      total_jvmconf += WbTime::get_duration(jvmconf_start_t, jvminit_start_t);
      total_logsync += WbTime::get_duration(logsync_start_t, jvmtask_start_t);
      total_jvmtask += WbTime::get_duration(jvmtask_start_t, map_start_t);
    }

  
    logfile.close();
  }

  cout << total_yarn_child / num_yarn_child << endl
      << total_ugi / num_yarn_child << endl
      << total_metrics / num_yarn_child << endl
      << total_cred / num_yarn_child << endl
      << total_jvmconf / num_yarn_child << endl
      << total_logsync / num_yarn_child << endl
      << total_jvmtask / num_yarn_child << endl;

  return 0;
}
