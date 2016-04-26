CFLAGS=-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy \
  -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op \
  -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast \
  -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion \
  -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default \
  -Wundef -Wno-unused -g

mitra: main.cpp
	g++ $(CFLAGS) $< -o $@

build: mitra

clean:
	rm -f vgcore* mitra

test:
	for number in 3 4 5 6 7 8 ; do \
	  echo $$number && \
	  time ./mitra < test/in$$number > out && \
	  colordiff out test/out$$number; \
	done

.PHONY: build clean test
