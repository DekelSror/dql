



components= $(addprefix $(PWD)/,$(subst .c,.out, $(shell ls -f | grep ".*.c")))



all: $(components)

$(PWD)/%.out:
	gcc -g -Wextra -Wall $*.c -I../../framework/_include -L../../framework/ -ldql -lpthread -lrt -o $*.out


test:
	@ echo $(components)
