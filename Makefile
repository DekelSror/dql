

# to fully automate, ll_modules needs to scan the root dir for modules
# notice that the format has to ne unified

ll_modules = mtsq container_q matrix simple_allocator tpool

iflags = $(addprefix -I., $(ll_modules))

compilerFlags = -L./libs -Wl,-rpath=./libs \
-pedantic-errors -Wextra -Wall -std=c99 \
$(iflags) -Iinclude \

debugFlags = -g -DDEBUG -Og

ifeq ($(debug), 1)
compilerFlags += $(debugFlags)
else
endif

ll_objs = $(addprefix objs/, $(addsuffix .o, $(ll_modules)))
ll_tests = $(addprefix tests/, $(addsuffix .out, $(ll_modules)))
libll = libs/libll.so


test: $(ll_tests)

tests/%.out: $(libll)
	gcc $(compilerFlags) lolevel/$*/$*_test.c objs/$*.o -o $@ -lpthread -lll

$(libll): $(ll_objs)
	gcc $(compilerFlags) -shared objs/*.o -o $@

objs/%.o:
	gcc $(compilerFlags) -c -fPIC lolevel/$*/$*.c -o $@

clean:
	rm -f objs/* libs/* tests/*