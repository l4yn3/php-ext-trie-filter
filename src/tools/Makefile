#
all: trie_filter
CC=gcc
PROGRAM_FILES=trie_filter.c
CFLAGS=-Wall 
CPPFLAGS=-I/usr/local/include
LDFLAGS=-L/usr/local/lib
LIBS=-ldatrie -liconv

trie_filter: trie_filter.c
	$(CC) -o trie_filter $(PROGRAM_FILES) $(CPPFLAGS) $(LDFLAGS) $(LIBS) $(CFLGAS)
clean: trie_filter
	rm -f trie_filter
