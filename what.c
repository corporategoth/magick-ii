#include <stdio.h>

void main(int argc, char **argv)
{
    unsigned char c;
    int i, sequence;
    FILE *in;

    for (i=1; i<argc; i++)
    {
	if ((in = fopen(argv[i], "r")) == NULL)
	{
	    fprintf(stderr, "Cannot open file: %s\n", argv[i]);
	}
	else
	{
	    printf("%s:\n", argv[i]);
	    sequence = 0;
	    for (c=fgetc(in); !feof(in); c=fgetc(in))
	    {
		if (c >= 32 && c <= 126 && sequence == 4)
		{
		    printf("%c", c);
		}
		else if ((c == '@' && sequence == 0) ||
		    (c == '(' && sequence == 1) ||
		    (c == '#' && sequence == 2))
		{
		    sequence++;
		}
		else if (c == ')' && sequence == 3)
		{
		    sequence++;
		    printf("        ");
		}
		else
		{
		    if (sequence == 4)
			printf("\n");
		    sequence = 0;
		}
	    }
	    fclose(in);
	}
    }
}
