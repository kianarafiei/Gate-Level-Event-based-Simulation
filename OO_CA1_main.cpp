#include "OO_CA1_source.cpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main() {
  ifstream verilog_file("gatelevel.v");
  ifstream test_bench_file("test_bench.txt");
  vector<Wire *> wires;
  vector<And *> and_gates;
  vector<Or *> or_gates;
  vector<Inverter *> inverter_gates;
  vector<InputEvent> input_events;

  string A, B;
  Manager manager(wires, and_gates, or_gates, inverter_gates, input_events, A,
                  B);
  manager.read_tst_file(test_bench_file);
  manager.read_verilog_file(verilog_file);
  manager.run();

  return 0;
}