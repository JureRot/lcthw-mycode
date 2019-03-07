CFLAGS=-g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)
LIBS=-ldl $(OPTLIBS)
PREFIX?=/usr/local

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SRC=$(wildcard tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

TARGET=build/libex29.a #we set the name of the lib here
SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

# The Target Build
all: $(TARGET) $(SO_TARGET) tests

dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@
$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

build:
	@mkdir -p build
	@mkdir -p bin

# The Unit Tests
.PHONY: tests
#tests: CFLAGS += $(TARGET)
#tests: $(TESTS)
tests:
	@echo $(TEST_SRC)
	@echo $(TESTS)
	cc -Wall -g -DNDEBUG  $(TEST_SRC) -ldl -o $(TESTS) #we build and link the tests
	$(TESTS) $(SO_TARGET) print_a_message "hello there" | true
	$(TESTS) $(SO_TARGET) uppercase "hello there" | true
	$(TESTS) $(SO_TARGET) lowercase "HELLO tHeRe" | true
	$(TESTS) $(SO_TARGET) fail_on_purpose "i fail" | true
	$(TESTS) $(SO_TARGET) fail_on_purpose | true
	$(TESTS) $(SO_TARGET) asdfasdf "asdfasdf" | true
	$(TESTS) ./build/libex.so print_a_message "i fail" | true
	#sh ./tests/runtests.sh
	#THIS IS AN UGLY WAY OF RUNNING TESTS BUT IT WORKS FOR NOW (| true is there to keep going and not stop on first error)

# The Cleaner
clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

# The Install
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# The Checker
check:
	@echo Files with potentially dangerous functions.
	@egrep '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|r?index[^.]|a?sn?printf|byte_)' $(SOURCES) || true
