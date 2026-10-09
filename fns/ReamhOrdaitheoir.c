#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Rule Rule;

struct Rule
{
	char before[64];
	char after[64];
};

Rule rules[64];
int curr_rule = 0;

void new_rule(const char* before, const char* after)
{
	strcpy(rules[curr_rule].before, before);
	strcpy(rules[curr_rule].after, after);
	curr_rule++;
};

void substitute(const char* line, char* out)
{
	char buffer[2048];
	strcpy(buffer, line);

	for (int i = 0; i < curr_rule; i++)
	{
		char temp[2048] = {0};
		char* pos = buffer;
		char* found;
		int before_len = strlen(rules[i].before);
		int after_len = strlen(rules[i].after);

		while ((found = strstr(pos, rules[i].before)) != NULL)
		{
			strncat(temp, pos, found - pos);
			strcat(temp, rules[i].after);
			pos = found + before_len;
		}

		strcat(temp, pos);
		strcpy(buffer, temp);
	}

	strcpy(out, buffer);
}

void process(FILE* in, FILE* out)
{
	char line[1024];
	char result[2048];

	while (fgets(line, sizeof(line), in))
	{
		substitute(line, result);
		fputs(result, out);
	}
}

void transpile(FILE *in, FILE *out)
{
	fprintf(out, "#include <stdio.h>\n");
	fprintf(out, "#include <stdlib.h>\n\n");

	char line[1024];
        char result[2048];
	while (fgets(line, sizeof(line), in)) 
	{
        	substitute(line, result);
        	fputs(result, out);
	}
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		fprintf(stderr, "ba cheart: %s ionchur.cf aschur\n", argv[0]);
		return 1;
	}

	//C lang

	//Primitive Types
	new_rule("slan", "int");
	new_rule("snamh", "float");
	new_rule("litir", "char");
	new_rule("beirte", "double");
	new_rule("gearr", "short");
	new_rule("fada", "long");

	new_rule("ceal", "NULL");

	//Modifyers
	new_rule("siniu", "signed");
	new_rule("gansiniu", "unsigned");

	//Conditions
	new_rule("ma ", "if ");
	new_rule("ma(", "if(");
	new_rule("eile ", "else ");
	new_rule("cur ", "return ");
	new_rule("cur;", "return;");
	new_rule("lean", "continue");
	new_rule("dean ", "do ");
	new_rule("dean(", "do(");
	new_rule("teigh", "goto");
	new_rule("slat", "switch");
	new_rule("cas", "case");
	new_rule("bunaigh", "default");
	new_rule("bris", "break");

	//Loops
	new_rule("tamall ", "while ");
	new_rule("tamall(", "while(");
	new_rule("faoi ", "for ");
	new_rule("faoi(", "for(");

	//Fancy
	new_rule("statach ", "static ");
	new_rule("seacht ", "extern ");

	//Data Structures
	new_rule("deant ", "struct ");
	new_rule("comh ", "union ");
	new_rule("euimh", "enum");

	new_rule("spleodrach", "volatile");
        new_rule("uath ", "auto ");
        new_rule("buan ", "const ");
        new_rule("folamh", "void");
	new_rule("clar", "register");
	new_rule("meid(", "sizeof(");
	new_rule("sortson", "typedef");

	new_rule("alp!", "#include");
	new_rule("priomh", "main");

	//<stdio.h>
	new_rule("<cdnio>", "<stdio.h>");
	new_rule("clof", "printf");
	new_rule("scanf", "scanf");
	new_rule("fclof", "fprintf");
	new_rule("fscanf", "fscanf");
	new_rule("faighlitir", "getchar");
	new_rule("cuirlitir", "putchar");
	new_rule("faighs", "gets");
	new_rule("cuirs", "puts");
	new_rule("foscail", "fopen");
	new_rule("fdun", "fclose");
	new_rule("fleigh", "fread");
	new_rule("fscriobh", "fwrite");
	new_rule("ffaighc", "fgetc");
	new_rule("fcuirc", "fputc");
	new_rule("ffaighs", "fgets");
	new_rule("fcuirs", "fputs");
	new_rule("florg", "fseek");
	new_rule("finis", "ftell");
	new_rule("athgoin", "rewind");
	new_rule("bain", "remove");
	new_rule("athainmnigh", "rename");

	//<stdlib.h>
	new_rule("cudail", "malloc");
	new_rule("saor", "free");

	const char* input_file = argv[1];
	const char* output_file = argv[2];

	char temp_c[256];
	snprintf(temp_c, sizeof(temp_c), "/tmp/_build_%d.c", getpid());

	FILE* in = fopen(input_file, "r");
	FILE* out = fopen(temp_c, "w");

	if (!in)
	{
    		fprintf(stderr, "Nil an comhad: %s\n", input_file);
        	return 1;
	}

	transpile(in, out);
    	fclose(in);
   	fclose(out);

   	char cmd[512];
   	snprintf(cmd, sizeof(cmd), "gcc %s -o %s", temp_c, output_file);

   	int result = system(cmd);

   	remove(temp_c);

   	if (result != 0)
	{
        	fprintf(stderr, "Compilation failed\n");
        	return 1;
   	}

	return 0;
}
