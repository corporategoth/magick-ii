#include <stdio.h>

void main(int argc, char **argv)
{
    char c;
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
		if (c=='@' && sequence == 0 ||
		    c=='(' && sequence == 1 ||
		    c=='#' && sequence == 2)
		    sequence++;
		if (c==')' && sequence == 3)
		{
		    sequence++;
		    printf("        ");
		}
		if (c==0 && sequence == 4)
		{
		    sequence = 0;
		    printf("\n");
		}
		if (sequence == 4)
		    printf("%c", c);
	    }
	    fclose(in);
	}
    }
}
