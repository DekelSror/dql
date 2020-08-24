dir_names= $(patsubst _%,,$(subst /,, $(shell ls -d */)))
headers=$(addprefix  $(PWD)/_include/, $(addsuffix .h, $(dir_names)))
modules=$(addprefix  $(PWD)/_modules/, $(addsuffix .o, $(dir_names)))
unit_tests=$(addprefix  $(PWD)/_tests/, $(addsuffix _test.out, $(dir_names)))


all: $(unit_tests) $(modules) $(headers) $(PWD)/libdql.so

$(PWD)/_tests/%.out: $(PWD)/libdql.so
	@echo making test $*
	gcc -g -Wextra -Wall -I./_include -I. \
	$(PWD)/$(subst _test,,$*)/$*.c \
	-o $@ \
	-L$(PWD) -Wl,-rpath=$(PWD) \
	 -ldql \
	-lpthread

$(PWD)/%.so: $(modules)
	@echo making lib $*
	@gcc -shared -o $@ $(modules)


$(PWD)/_modules/%.o: $(headers)
	@echo compiling $*
	@gcc -g -Wextra -Wall -fpic -I./_include -I. -c \
	-o $@ \
	$(PWD)/$*/$*.c


$(PWD)/_include/%.h: 
	@echo making link $*
	ln -sf $(PWD)/$*/$*.h $@



.PHONY: clean

clean:
	rm libdql.so
	rm _tests/*
	rm _modules/*
	rm _include/*
