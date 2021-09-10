#######################################################
###				CONFIGURATION
#######################################################

STA_DIR = stack
QUE_DIR = queue

TST_DIR = test
COM_DIR = common

ADT_DIRS = $(STA_DIR) $(QUE_DIR)

CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c99 -Wstrict-prototypes -Wmissing-prototypes -fPIC\
		 -Wunreachable-code -Wconversion -Wmissing-declarations -Wno-unused-parameter -Wshadow -Wbad-function-cast -O3 -g
CPPFLAGS	= -I ${TST_DIR}

TESTS_EXEC 	= test_stack test_queue

#######################################################
###				MAKE DEFAULT COMMAND
#######################################################

.PHONY: all help build test vtest clean docs
all: help

#######################################################
###				MAKE INSTRUCTIONS / HELP
#######################################################

help:
	@echo -e Available commands:'\n' \
		'\t' make help:'\t'  \ \ Displays this screen								'\n' \
		'\t' make build:'\t' \ \ Compiles every .c ADT sources into .o				'\n' \
		'\t' make test:'\t' \ \ Builds sources and tests, then execute the test	'\n' \
		'\t' make vtest:'\t' \ \ Executes tests with Valgrind\'s memory analyse only'\n' \
		'\t' make clean:'\t' \ \ Removes all the .o  and test executables			'\n' \
		'\t' make \<test_name\>: Builds \<test_name\> only						'\n' \
								'\n' \
		Commands details can be found in the README

#######################################################
###				MAKE BUILD
#######################################################

prebuild:
	@echo Starting building...

build: prebuild
	@echo building objects...
	@for dir in $(ADT_DIRS); do \
		${CC} $(CFLAGS) $(dir)/*.c -o $(dir:%.c=%.o); \ #FIXME
	done
	@echo Building complete.

#######################################################
###				MAKE TEST
#######################################################

test: $(TESTS_EXEC)
ifneq ($(TESTS_EXEC),)
	@echo Starting tests...
	@for e in $(TESTS_EXEC); do \
		./$${e}; echo; \
	done
	@printf "\nTests complete.\n";
else
	@echo No test available
endif

#######################################################
###				MAKE TEST WITH VALGRIND
#######################################################

vtest: $(TESTS_EXEC)
ifneq ($(TESTS_EXEC),)
	@echo Starting tests...
	@for e in $(TESTS_EXEC); do \
		echo ======= $${e} =======; \
		filename=$$(echo ./$(TST_DIR)/$${e} | cut -d_ -f2); \
		printf "TESTED FILE:\t$$filename.c\n"; \
		valgrind --log-fd=1 ./$${e} \
		| grep "TESTS SUMMARY:\|ERROR SUMMARY:\|total heap usage:" \
		| $(VALGRIND_AWK) \
	done
	@printf "\nTests complete.\n";
else
	@echo No test available
endif

#######################################################
###				MAKE CLEAN
#######################################################

clean:
	@echo Starting cleanup...
	@find . -type f -name '*.o' -delete
	@rm -rf ./$(TESTS_EXEC)
	@echo Cleanup complete.

#######################################################
###				TEST EXECUTABLES
#######################################################

test_stack:	./$(TST_DIR)/test_stack.o ./$(TST_DIR)/common_tests_utils.o ./$(STA_DIR)/stack.o
	${CC} $(CFLAGS) $^ -o $@

test_queue:	./$(TST_DIR)/test_queue.o ./$(TST_DIR)/common_tests_utils.o ./$(QUE_DIR)/queue.o
	${CC} $(CFLAGS) $^ -o $@

#######################################################
###				OBJECTS FILES
#######################################################

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

#######################################################
###				EXTRAS
#######################################################

VALGRIND_AWK = \
awk '{	\
	if (match( $$0, /TESTS.*/)) \
		printf "%s\n", $$0; \
	else \
		for(i=2;i<=NF;i++) { \
			if (match($$((i+1)), /allocs/) && $$i > $$((i+2))) \
				   printf "\x1B[31m%s \x1b[0m", $$i; \
			else if (match($$i, /[1-9]+$$/) && match($$((i+1)), /errors/)) \
				   printf "\x1B[31m%s \x1b[0m", $$i; \
			else if (match($$i, /[1-9]+$$/) && match($$((i+1)), /contexts/)) \
				   printf "\x1B[31m%s \x1b[0m", $$i; \
			else if (match($$i, /[0-9]+$$/)) \
				printf "\x1B[32m%s \x1b[0m", $$i; \
		   	else if (match($$i, /ERROR/)) \
				printf "\n%s ", $$i; \
			else if (match($$i, /total/)) \
				printf ""; \
			else if (match($$i, /heap/)) \
				printf "HEAP "; \
			else if (match($$i, /usage:/)) \
				printf "USAGE: \t"; \
			else if (match($$i, /frees\,/)) \
				{printf "frees", $$i; break;}\
			else if (match($$i, /contexts/)) \
				{printf "%s", $$i; break;}\
			else \
				printf "%s ", $$i; \
			} \
	}'; \
echo; echo;
