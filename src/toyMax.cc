// File: toyMax.cc
// calculate maximum value in given array
// Jovana Djokovic (djokovic@princeton.edu)
// *Princeton University ILA template*

#include <toyMax.h>
#include <ilang/ilang++.h>

namespace ilang {

namespace toyMax {

Ila GetToymaxIla(const std::string& toyModel) {
  auto m = Ila("toyModel");

  // define interface and architectural states
  auto mode    = m.NewBvInput("mode", 1);
  auto addr_in = m.NewBvInput("addr_in", 8);
  auto data_in = m.NewBvInput("data_in", 8);

  // memory 160 byte
  auto mem = m.NewMemState("mem", 8, 8);

  // registers
  auto start_addr = m.NewBvState("start_addr", 8);
  auto array_len  = m.NewBvState("array_len", 8);
  auto result     = m.NewBvState("result", 8);

  // child! :)
  auto child_valid = m.NewBvState("child_valid", 1);
  auto child_state = m.NewBvState("child_state", 2);

  // define ILA valid function
  m.SetValid(addr_in >= 0x00 & addr_in < 0xA4);
  m.AddInit(start_addr == 0);
  m.AddInit(child_valid == 0);

  // define instruction - decode and state update functions
  { // give array start address
    auto instr = m.NewInstr("START_ADDR");
    instr.SetDecode((mode == 1) & (addr_in == 0xA1));
    instr.SetUpdate(start_addr, data_in);

    // precaution
    instr.SetUpdate(array_len, array_len);
  }

  { // give array length
    auto instr = m.NewInstr("WRITE_LEN");
    instr.SetDecode((mode == 1) & (addr_in == 0xA2));
    instr.SetUpdate(array_len, data_in);

    // precaution
    instr.SetUpdate(start_addr, start_addr);
  }

  { // start computing max
    auto instr = m.NewInstr("FIND_MAX");
    instr.SetDecode((mode == 1) & (addr_in == 0xA3));

    instr.SetUpdate(child_valid, BvConst(1,1));
    instr.SetUpdate(child_state, BvConst(0,2));

    AddChild(m);

    // precaution
    instr.SetUpdate(array_len, array_len);
    instr.SetUpdate(start_addr, start_addr);
  }

  { // store data into memory
    auto instr = m.NewInstr("STORE_DATA");
    instr.SetDecode(mode == 1);

    instr.SetUpdate(mem, Store(mem, addr_in, result));

    // precaution
    instr.SetUpdate(array_len, array_len);
    instr.SetUpdate(start_addr, start_addr);
    instr.SetUpdate(result, result);
  }

  return m;
}

}; // namespace toyProj

}; // namespace ilang

