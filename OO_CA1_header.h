#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct InputEvent {
  int time;
  string Ainp;
  string Binp;
};

class Wire {
public:
  Wire(char input_value) : value(input_value) {}
  void set_value(char new_value) { value = new_value; }
  char get_value() { return value; }

private:
  char value;
};

class Gate {
public:
  virtual void evl() = 0;
  virtual ~Gate() {}
  string get_name() { return gate_name; }
  int get_delay() { return gate_delay; }
  void set_delta(int delta) { delta_time = delta; };
  int get_deta_time() { return delta_time; }
  void reset() { has_changed = true; }

protected:
  bool has_changed = true;
  int gate_delay;
  string gate_name;
  int delta_time;
};

class And : public Gate {
public:
  And(Wire *in1, Wire *in2, Wire *out, string name, int delay)
      : i1(in1), i2(in2), o(out) {
    gate_name = name;
    gate_delay = delay;
  }
  void evl() override;

  Wire *get_output() { return o; }
  void set_ios(Wire *input1, Wire *input2, Wire *output) {
    i1 = input1;
    i2 = input2;
    o = output;
  }

private:
  Wire *i1;
  Wire *i2;
  Wire *o;
};

class Or : public Gate {
public:
  Or(Wire *in1, Wire *in2, Wire *out, string name, int delay)
      : i1(in1), i2(in2), o(out) {
    gate_name = name;
    gate_delay = delay;
  }
  void evl() override;
  Wire *get_output() { return o; }
  void set_ios(Wire *input1, Wire *input2, Wire *output) {
    i1 = input1;
    i2 = input2;
    o = output;
  }

private:
  Wire *i1;
  Wire *i2;
  Wire *o;
};

class Inverter : public Gate {
public:
  Inverter(Wire *in, Wire *out, string name, int delay) : i1(in), o(out) {
    gate_name = name;
    gate_delay = delay;
  }
  void evl() override;
  Wire *get_output() { return o; }
  void set_ios(Wire *input1) { i1 = input1; }
  Wire *get_input() { return i1; }
  void set_ios(Wire *input1, Wire *output) {
    i1 = input1;
    o = output;
  }

private:
  Wire *i1;
  Wire *o;
};

class Manager {
public:
  Manager(vector<Wire *> input_wires, vector<And *> input_and_gates,
          vector<Or *> input_or_gates, vector<Inverter *> input_inverter_gates,
          vector<InputEvent> input_events, string A, string B)
      : wires(input_wires), and_gates(input_and_gates),
        or_gates(input_or_gates), inverter_gates(input_inverter_gates),
        inp_events(input_events), A_input(A), B_input(B) {}
  void read_verilog_file(ifstream &file);

  void read_tst_file(ifstream &file1);
  void run();
  void write_output(int time, ofstream &myFile);
  void manage_assigns();
  void set_delta_time();

private:
  vector<Wire *> wires;
  vector<And *> and_gates;
  vector<Or *> or_gates;
  vector<Inverter *> inverter_gates;
  vector<Gate *> gates;
  vector<InputEvent> inp_events;
  string A_input, B_input;
};