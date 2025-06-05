#include "OO_CA1_header.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void And::evl()
{
  while (has_changed)
  {
    if (i1->get_value() == '1' and i2->get_value() == '1')
    {
      if (o->get_value() == '1')
        has_changed = false;
      o->set_value('1');
    }
    else if (i1->get_value() == '0' or i2->get_value() == '0')
    {
      if (o->get_value() == '0')
        has_changed = false;
      o->set_value('0');
    }
    else
    {
      if (o->get_value() == 'X')
        has_changed = false;
      o->set_value('X');
    }
  }
}

void Or::evl()
{

  while (has_changed)
  {
    if (i1->get_value() == '0' && i2->get_value() == '0')
    {
      if (o->get_value() == '0')
        has_changed = false;
      o->set_value('0');
    }
    else if (i1->get_value() == '1' || i2->get_value() == '1')
    {
      if (o->get_value() == '1')
        has_changed = false;
      o->set_value('1');
    }
    else
    {
      if (o->get_value() == 'X')
        has_changed = false;
      o->set_value('X');
    }
  }
}

void Inverter::evl()
{
  while (has_changed)
  {
    if (i1->get_value() == '1')
    {
      if (o->get_value() == '0')
        has_changed = false;
      o->set_value('0');
    }
    else if (i1->get_value() == '0')
    {
      if (o->get_value() == '1')
        has_changed = false;
      o->set_value('1');
    }
    else
    {
      if (o->get_value() == 'X')
        has_changed = false;
      o->set_value('X');
    }
  }
}

void Manager::read_verilog_file(ifstream &file)
{
  if (!file.is_open())
  {
    cerr << "Could not open the file!\n";
    return;
  }

  string line;

  // Regular expressions
  regex module_regex(R"(module\s+(\w+)\s*\(([^)]*)\);)");
  regex input_output_regex(
      R"(input\s*\[?[0-9]*:?[0-9]*\]?\s*([\w, ]+)|output\s*([\w, ]+))");
  regex wire_regex(R"(wire\s*\[?[0-9]*:?[0-9]*\]?\s*([\w, ]+);)");
  regex and_gate_regex(R"(and#(\d+)\s+(\w+)\(([^,]+),([^,]+),([^)]+)\);)");
  regex or_gate_regex(R"(or#(\d+)\s+(\w+)\(([^,]+),([^,]+),([^)]+)\);)");
  regex not_gate_regex(R"(not#(\d+)\s+(\w+)\(([^,]+),([^)]+)\);)");
  regex assign_regex(R"(assign\s+(\w+)\s*=\s*([\w\[\]]+);)");
  for (int i = 0; i < 20; i++)
  {
    wires.push_back(new Wire('X'));
  }
  // Reading file line by line
  while (getline(file, line))
  {
    smatch match;

    if (regex_search(line, match, and_gate_regex))
    {

      int index = stoi(match[4].str().substr(2));
      Wire *inp1_wire = wires[index];

      index = stoi(match[5].str().substr(2));
      Wire *inp2_wire = wires[index];

      index = stoi(match[3].str().substr(2));
      Wire *output = wires[index];

      string gate_name = match[2];
      int delay = stoi(match[1]);

      And *new_and = new And(inp1_wire, inp2_wire, output, gate_name, delay);
      and_gates.push_back(new_and);
      gates.push_back(new_and);

      continue;
    }

    if (regex_search(line, match, or_gate_regex))
    {
      int index = stoi(match[4].str().substr(2));
      Wire *inp1_wire = wires[index];

      index = stoi(match[5].str().substr(2));
      Wire *inp2_wire = wires[index];

      index = stoi(match[3].str().substr(2));
      Wire *output = wires[index];

      Or *new_or =
          new Or(inp1_wire, inp2_wire, output, match[2], stoi(match[1]));
      or_gates.push_back(new_or);
      gates.push_back(new_or);
      continue;
    }

    if (regex_search(line, match, not_gate_regex))
    {
      int index = stoi(match[4].str().substr(2));
      Wire *inp1_wire = wires[index];

      index = stoi(match[3].str().substr(2));
      Wire *output = wires[index];

      Inverter *new_inverter =
          new Inverter(new Wire(A_input[0]), output, match[2], stoi(match[1]));
      inverter_gates.push_back(new_inverter);
      gates.push_back(new_inverter);
      continue;
    }
  }
}

void Manager::read_tst_file(ifstream &file1)
{
  string line;

  int time;
  while (getline(file1, line))
  {
    if (line.empty() || line[0] != '#')
      continue;

    istringstream iss(line);
    char hash;
    string a, b;
    iss >> hash >> time >> a >> b;
    InputEvent event;
    event.Ainp = a;
    event.Binp = b;
    event.time = time;
    inp_events.push_back(event);
  }
}

void Manager::manage_assigns()
{

  wires[16]->set_value(B_input[0]);
  wires[17]->set_value(A_input[0]);
  wires[18]->set_value(B_input[1]);
  wires[19]->set_value(A_input[1]);

  and_gates[0]->set_ios(wires[16], wires[0], wires[4]);
  and_gates[1]->set_ios(wires[17], wires[1], wires[5]);
  and_gates[2]->set_ios(wires[18], wires[2], wires[6]);
  and_gates[3]->set_ios(wires[19], wires[3], wires[7]);
  and_gates[4]->set_ios(wires[9], wires[6], wires[10]);
  and_gates[5]->set_ios(wires[9], wires[7], wires[11]);
  or_gates[0]->set_ios(wires[5], wires[4], wires[8]);
  or_gates[1]->set_ios(wires[10], wires[4], wires[12]);
  or_gates[2]->set_ios(wires[5], wires[11], wires[13]);
  or_gates[3]->set_ios(wires[12], wires[13], wires[14]);
  inverter_gates[0]->set_ios(wires[17], wires[0]);
  inverter_gates[1]->set_ios(wires[16], wires[1]);
  inverter_gates[2]->set_ios(wires[19], wires[2]);
  inverter_gates[3]->set_ios(wires[18], wires[3]);
  inverter_gates[4]->set_ios(wires[8], wires[9]);
  inverter_gates[5]->set_ios(wires[14], wires[15]);
}

void Manager::set_delta_time()
{

  for (int i = 0; i < 4; i++)
  {
    and_gates[i]->set_delta(inverter_gates[0]->get_delay());
    inverter_gates[i]->set_delta(0);
  }
  or_gates[0]->set_delta(inverter_gates[0]->get_delay() +
                         and_gates[0]->get_delay());
  inverter_gates[4]->set_delta(or_gates[0]->get_deta_time() +
                               and_gates[0]->get_delay());
  and_gates[4]->set_delta(inverter_gates[4]->get_deta_time() +
                          inverter_gates[0]->get_delay());
  and_gates[5]->set_delta(and_gates[4]->get_deta_time());
  or_gates[1]->set_delta(and_gates[4]->get_deta_time() +
                         and_gates[4]->get_delay());
  or_gates[2]->set_delta(or_gates[1]->get_deta_time());
  or_gates[3]->set_delta(or_gates[2]->get_deta_time() +
                         or_gates[2]->get_delay());
  inverter_gates[5]->set_delta(or_gates[3]->get_deta_time() +
                               or_gates[2]->get_delay());
}

void Manager::run()
{
  ofstream myFile("result.txt");
  int current_time = 0;
  set_delta_time();

  for (int k = 0; k < inp_events.size(); k++)
  {

    int worst_case_delay =
        inverter_gates[0]->get_delay() + and_gates[0]->get_delay() +
        or_gates[0]->get_delay() + inverter_gates[4]->get_delay() +
        and_gates[4]->get_delay() + or_gates[1]->get_delay() +
        or_gates[3]->get_delay() + inverter_gates[5]->get_delay() +
        inp_events[0].time;

    for (int i = 0; i < worst_case_delay + 1; i++)
    {
      if (current_time == inp_events[k].time)
      {

        A_input = inp_events[k].Ainp;
        B_input = inp_events[k].Binp;
        manage_assigns();

        for (auto gate : gates)
        {
          gate->reset();
        }
      }

      for (int j = 0; j < gates.size(); j++)
      {
        if (current_time - inp_events[k].time == gates[j]->get_deta_time())
        {
          gates[j]->evl();
        }
      }
      current_time++;
    }
    write_output(current_time, myFile);
  }
  myFile.close();
}

void Manager::write_output(int current_time, ofstream &myFile)
{

  if (myFile.is_open())
  {

    myFile << "# " << current_time << " "
           << "gt : " << or_gates[2]->get_output()->get_value() << endl;
    myFile << "# " << current_time << " "
           << "lt : " << or_gates[1]->get_output()->get_value() << endl;
    myFile << "# " << current_time << " "
           << "eq : " << inverter_gates[5]->get_output()->get_value() << endl;
  }
  else
  {
    cerr << "Unable to open file." << endl;
  }
}