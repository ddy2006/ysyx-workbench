compile:
	verilator --cc two_wire.v --exe --trace tb.cpp
	make -j -C obj_dir -f Vtwo_wire.mk Vtwo_wire
sim: compile
	./obj_dir/Vtwo_wire
draw: sim
	gtkwave wave.vcd
clean:
	rm -rf obj_dir