// File: childMax.cc ----------------------------------------------------------
// child executes steps of FSM to compute maximum for parent toyMax.cc
// Jovana Djokovic (djokovic@princeton.edu)

#include <toyMax.h>
#include <ilang/ilang++.h>

namespace ilang {

namespace toyMax {

void AddChild(Ila& m) {
    auto child = m.NewChild("childMax");

    child.SetValid(m.state("child_valid") == BvConst(1, 1));
    child.SetFetch(m.state("child_state"));

    // some variables
    auto counter = child.NewBvState("counter", 8);
    auto curr_max = child.NewBvState("curr_max", 8);
    auto pot_max = child.NewBvState("pot_max", 8);

    // initialize variables
    child.AddInit(counter == 0);
    child.AddInit(curr_max == 0);
    child.AddInit(pot_max == 0);

    { // START
	auto instr = child.NewInstr("START");
	instr.SetDecode(m.state("child_state") == BvConst(0, 2));
	
	// move first index to variable "current maximum" (arbitrary)
	instr.SetUpdate(curr_max, Load(m.state("mem"), m.state("start_addr")));
	// update index number (counter)
	instr.SetUpdate(counter, counter + 1);

	// precaution
	instr.SetUpdate(pot_max, pot_max);
	// next state
	instr.SetUpdate(m.state("child_state"), BvConst(1, 2));
    }
    
    { // LOAD & COMPARE
	auto instr = child.NewInstr("COMPARE");    
        instr.SetDecode(m.state("child_state") == BvConst(1, 2));                                                              
        
	// load next index to variable "potential maximum" to be tested
	instr.SetUpdate(pot_max, Load(m.state("mem"), 
				m.state("start_addr") + counter));
        // update index number (counter)         
        instr.SetUpdate(counter, counter + 1);
	// compare potential max to current maximum and replace with winner
        instr.SetUpdate(curr_max, Ite(curr_max > pot_max, curr_max, pot_max));
        // next state depends on counter, creating loop
	instr.SetUpdate(m.state("child_state"), 
			Ite(counter == m.state("array_len"), 
				BvConst(2, 2), BvConst(1, 2)));
    }

    { // STORE
	 auto instr = child.NewInstr("STORE");
        instr.SetDecode(m.state("child_state") == BvConst(2, 2));
	
	// put current maximum into result
	instr.SetUpdate(m.state("result"), curr_max);
	// turn off child 
	instr.SetUpdate(m.state("child_valid"), BvConst(0, 1));
    }

}

}; // namespace toyMax

}; // namespace ilang
