

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "datrie/trie.h"

#define MAX_WORD_LEN 1024

void show_help(const char * prog_name) {
	printf("Usage: %s [OPTION]\n", prog_name);
	printf("Options:\n");
	printf("  -h, display this help and exit\n");
	printf("  -i, input worlds file\n");
	printf("  -o, output worlds tree file\n");
}

int main(int argc, char *argv[]) {
	int result = 0;
	char * infile_path = NULL;
	char * outfile_path = NULL;
	int param;

	if (argc <= 1) {
		show_help(argv[0]);
		return 0;
	}

	while ((param = getopt(argc, argv, "hi:o:")) != -1 ) {
		if (param == 'h' ) {
			show_help(argv[0]);
			return 0;
		} else if ( param == 'i' ) {
			infile_path = optarg;
		} else if ( param == 'o' ) {
			outfile_path = optarg;
		} else {
			show_help(argv[0]);
			return 0;
		}
	}

	if (infile_path == NULL || outfile_path == NULL) {
		show_help(argv[0]);
		return 0;
	}

	FILE *fp = fopen(infile_path, "r");
	if (!fp) {
		fprintf(stderr, "Failed to read words file %s\n", infile_path);
		result = 1;
		goto end_prog;
	}

	AlphaMap *alpha_map = alpha_map_new();
	if (!alpha_map) {
		fprintf(stderr, "call alpha_map_new() failed\n");
		result = 1;
		goto end_prog;
	}

	if (alpha_map_add_range(alpha_map, 0x00, 0xff) != 0) {
		fprintf(stderr, "call alpha_map_new() failed\n");
		result = 1;
		goto end_prog;
	}

	Trie *trie = trie_new(alpha_map);
	if (!trie) {
		fprintf(stderr, "call trie_new() failed\n");
		result = 1;
		goto end_prog;
	}

	int found = 0, len = 0;
	char *p = NULL;
	char line[MAX_WORD_LEN + 1];
	AlphaChar alpha_key[MAX_WORD_LEN];

	//read max MAX_WORD_LEN - 1
	while ((p = fgets(line, MAX_WORD_LEN, fp)) != NULL) {
		len = 0;
		while (*p && *p != '\n' && *p != '\r') {
			alpha_key[len] = (AlphaChar)(unsigned char)*p;
			p++;
			len++;
		}

		alpha_key[len] = TRIE_CHAR_TERM;

		if (!trie_store(trie, alpha_key, -1)) {
			fprintf(stderr, "failed to store: %s\n", line);
		} else {
			found++;
		}
	}

	if (0 == found || trie_save(trie, outfile_path)) {
		fprintf(stderr, "failed to save word tree file: %s\n", outfile_path);
		result = 1;
		goto end_prog;
	}

end_prog:
	if (fp) {
		fclose(fp);
	}

	exit(result);
}
