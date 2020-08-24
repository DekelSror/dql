dir_names= $(patsubst _%,,$(subst /,, $(shell ls -d */)))
headers=$(addprefix  $(PWD)/_include/, $(addsuffix .h, $(dir_names)))
modules=$(addprefix  $(PWD)/_modules/, $(addsuffix .o, $(dir_names)))
unit_tests=$(addprefix  $(PWD)/_tests/, $(addsuffix _test.out, $(dir_names)))
source_files=$(addsuffix .c,$(join $(addsuffix /,$(dir_names)),$(dir_names)))


all: $(unit_tests) $(modules)  $(PWD)/libdql.so $(headers)

$(PWD)/_tests/%.out: $(PWD)/libdql.so $(modules)
	@echo making test $*
	@gcc -g -Wextra -Wall -I./_include -I. \
	$(PWD)/$(subst _test,,$*)/$*.c \
	-o $@ \
	-L$(PWD) -Wl,-rpath=$(PWD) \
	 -ldql \
	-lpthread

$(PWD)/%.so: $(modules)
	@echo making lib $*
	@gcc -shared -o $@ $(modules)


$(PWD)/_modules/%.o: $(headers)
	@echo compiling $* "\n\n" $^ "\n\n"
	@gcc -g -Wextra -Wall -fpic -I./_include -I. -c \
	-o $@ \
	$(PWD)/$*/$*.c


$(PWD)/_include/%.h: 
	@echo making link $*
	ln -sf $(PWD)/$*/$*.h $@



.PHONY: clean erase

clean:
	rm _tests/*
	rm _modules/*

erase:
	rm _include/*
	rm libdql.so
