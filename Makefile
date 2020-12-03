name := marched_fractals
username := 
hostname := access.computing.clemson.edu

include_dir := include
src_dir := src
obj_dir := out

share_dir := /home/$(username)/Desktop
contents := $(include_dir) $(src_dir) Makefile

libs := m
configs := 

dir := $(shell basename "$(CURDIR)")
src := $(wildcard $(src_dir)/*.cpp)
objs := $(src:$(src_dir)/%.cpp=$(obj_dir)/%.o)

CXXFLAGS += -std=c++14 -Wall
CXXFLAGS += $(foreach dir,$(include_dir),-I$(dir))
LDFLAGS += $(foreach lib,$(libs),-l$(lib))
LDFLAGS += $(foreach config,$(configs),$(config))

all: $(obj_dir) $(name)
test: clear all run

run:
	@./$(name)
clear:
	@clear
clean:
	@- $(RM) $(obj_dir)/*.o
	@- $(RM) $(name)
dist:
	@tar --exclude="$(name).tar.gz" --exclude="out" --exclude="$(name)" -czvf $(name).tar.gz ../$(dir)
share:
	@scp -r $(contents) $(username)@$(hostname):$(share_dir)/$(dir)
shareout:
	@scp $(name) $(username)@$(hostname):$(share_dir)/$(name)

$(name): $(objs)
	@$(LINK.cc) $(objs) $(LDFLAGS) -o $(name)
$(obj_dir)/%.o: $(src_dir)/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
$(obj_dir):
	@mkdir -p $(obj_dir)