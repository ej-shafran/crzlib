lib_files=$(wildcard lib/*.h)
test_files=$(wildcard test/*.c)
test_outputs=$(patsubst test/%.c,build/%,$(test_files))

.PHONY: build
build: $(test_outputs)

build/%: test/%.c $(lib_files)
	mkdir -p build/
	gcc -I./lib -Wall -Wextra -Werror -pedantic -ggdb -o $@ $<

.PHONY: clean
clean:
	rm -rf build/

.PHONY: ci
ci: build
	for test in ./build/* ; do \
		valgrind $$test ; \
	done
