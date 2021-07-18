CXX := clang++
CXX_VERSION := -std=c++20 
MEMORY_CHECK_FLAGS := -O1 -g -fsanitize=address -fno-omit-frame-pointer
SOURCES := ./main.cc
LIBRARIES := -l SDL2

all: a.out

a.out: $(SOURCES)
	$(CXX) $(CXX_VERSION) $(LIBRARIES) \
		$(MEMORY_CHECK_FLAGS) \
		-o $@ $^ 

animation: $(SOURCES)
	$(CXX) $(CXX_VERSION) $(LIBRARIES) \
		-o $@ $^ 

run: animation
		./$<

check-memory: a.out
	ASAN_OPTIONS=symbolize=1 \
		ASAN_SYMBOLIZER_PATH=$(shell which llvm-symbolizer) \
		./$<

clean:
	rm -f a.out animation vgcore.*

.PHONY: all run clean
