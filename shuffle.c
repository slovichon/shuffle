/* $Id$ */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

__dead void	 usage(void);
int		 cmp(const void *, const void *);
void		 shuffle(FILE *);
char		*getline(FILE *);

int
main(int argc, char *argv[])
{
	FILE *fp;
	int ch;

	fp = NULL; /* gcc */
	while ((ch = getopt(argc, argv, "")) != -1)
		switch (ch) {
		default:
			usage();
			/* NOTREACHED */
		}
	argc -= optind;
	argv += optind;
	if (*argv == NULL)
		shuffle(stdin);
	else
		while (*argv != NULL) {
			if (strcmp(*argv, "-") == 0)
				fp = stdin;
			else {
				if ((fp = fopen(*argv, "r")) == NULL)
					err(1, "%s", *argv);
			}
			shuffle(fp);
		}
	exit(0);
}

#define INCR 1024

void
shuffle(FILE *fp)
{
	char **lines, **linesdup;
	size_t cur, max;
	char *ln;
	int i;

	/* XXX: predict nlines from st_siz */
	lines = NULL;
	cur = max = 0;
	while ((ln = getline(fp)) != NULL) {
		if (++cur >= max) {
			max += INCR;
			if ((linesdup = realloc(lines,
			    max * sizeof(*linesdup))) == NULL)
				err(1, "realloc");
		}
		lines[cur] = ln;
	}
	qsort(lines, cur, sizeof(*lines), cmp);
	for (i = 0; i < cur; i++)
		free(lines[i]);
	free(lines);
}

int
cmp(const void *a, const void *b)
{
	return (arc4random() % 3 - 1);
}

char *
getline(FILE *fp)
{
	size_t siz;
	char *p, *s;

	if ((p = fgetln(fp, &siz)) == NULL)
		return (NULL);
	if (p[siz - 1] != '\n') {
		if ((s = realloc(p, ++siz)) == NULL)
			return (NULL);
		p = s;
	} else {
		if ((p = strdup(p)) == NULL)
			return (NULL);
	}
	p[siz - 1] = '\0';
	return (p);
}

void
usage(void)
{
	extern char *__progname;

	fprintf(stderr, "usage: %s [file ...]\n", __progname);
	exit(1);
}
