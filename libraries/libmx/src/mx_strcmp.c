#include "../inc/libmx.h"

int mx_strcmp(const char *s1, const char *s2)
{ 
	if ((s1 && !s2) || (!s1 && s2)) return 1;
	else if (!s1 && !s2) return 0;

	int i = 0;
	while (s1[i] != '\0'
		&& s2[i] != '\0')
	{
			if (s1[i] != s2[i]) return s1[i] - s2[i];
		 	i++;
	}
	return s1[i] - s2[i];
}

int mx_strcmp_ic(const char *s1, const char *s2)
{ 
	int i = 0;
	while (s1[i] != '\0'
		&& s2[i] != '\0')
	{
		char c1 = mx_tolower(s1[i]);
		char c2 = mx_tolower(s2[i]);
		if (c1 != c2) 
			return c1 - c2;
		i++;
	}
	return mx_tolower(s1[i]) - mx_tolower(s2[i]);
}


