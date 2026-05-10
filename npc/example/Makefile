NVBOARD_HOME ?= /home/www37/workspace/ysyx/ysyx-workbench/nvboard  # 替换为你的NVBoard路径
VERILATOR = verilator

all: run

run:
    $(VERILATOR) -MMD --build -j 0 \
    --top-module top \
    --exe main.c \
    -I$(NVBOARD_HOME)/include \
    -LDFLAGS "-L$(NVBOARD_HOME)/build -lnvboard -lSDL2" \
    example.v
    ./obj_dir/Vtop

clean:
    rm -rf obj_dir