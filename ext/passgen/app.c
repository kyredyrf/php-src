#include "app.h"

#include "hash_value.h"
#include "rand48.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct app_t 
{
	char* use_char;
	int use_char_count;
	char* keyword;
	int encode_count;

	char* encode;
};

app_t* app_create()
{
	app_t* app;

	app = (app_t*)malloc(sizeof(*app));
	app->use_char = NULL;
	app->keyword = NULL;
	app->encode_count = 0;
	app->encode = NULL;

	return app;
}

void app_destroy(app_t* app)
{
	if (app != NULL)
	{
		free(app->encode);
		free(app);
	}
}

void app_parse_arguments(app_t* app, int argc, const char* argv[])
{
	int i;

	for (i = 0; i < argc; i++)
	{
		if (argv[i][0] == '-' || argv[i][0] == '/')
		{
			if (argv[i][1] == 'u' || strcmp(argv[i], "--use-char") == 0)
			{
				app_set_use_char(app, argv[++i]);
			}
			else if (argv[i][1] == 'k' || strcmp(argv[i], "--keyword") == 0)
			{
				app_set_keyword(app, argv[++i]);
			}
			else if (argv[i][1] == 'c' || strcmp(argv[i], "--count") == 0)
			{
				app_set_encode_count(app, atoi(argv[++i]));
			}
		}
	}
}

void app_set_use_char(app_t* app, const char* str)
{
	int i;
	char map[128];
	int length;
	char use_char[129];

	free(app->use_char);
	free(app->encode);
	app->encode = NULL;

	for (i = 0; i < (int)(sizeof(map) / sizeof(map[0])); i++)
		map[i] = 0;

	length = strlen(str);
	for (i = 0; i < length; i++)
		map[(int)str[i]] = 1;

	app->use_char_count = 0;
	for (i = 0; i < (int)(sizeof(map) / sizeof(map[0])); i++)
	{
		if (map[i] != '\0')
			use_char[app->use_char_count++] = i;
	}
	use_char[app->use_char_count] = '\0';

	app->use_char = (char*)malloc(app->use_char_count + 1);
	strcpy(app->use_char, use_char);
}

void app_set_keyword(app_t* app, const char* str)
{
	free(app->keyword);
	free(app->encode);
	app->encode = NULL;

	app->keyword = (char*)malloc(strlen(str) + 1);
	strcpy(app->keyword, str);
}

void app_set_encode_count(app_t* app, int count)
{
	free(app->encode);
	app->encode = NULL;

	app->encode_count = count;
}

const char* app_encode(app_t* app)
{
	if (app->encode == NULL)
	{
		if (app->use_char == NULL || app->keyword == NULL || app->encode_count <= 0)
		{
			app->encode = (char*)malloc(1);
			app->encode[0] = '\0';
		}
		else
		{
			rand48_t* r;
			char b[12];
			char* t;
			int i;

			r = rand48_create();

			sprintf(b, "%d", app->encode_count);

			t = (char*)malloc(strlen(app->keyword) + strlen(b));
			strcpy(t, app->keyword);
			strcat(t, b);

			rand48_set_seed(r, get_hash_value(t));

			app->encode = (char*)malloc(app->encode_count + 1);
			for (i = 0; i < app->encode_count; i++)
			{
				app->encode[i] = app->use_char[rand48_get_nonnegative_integer(r) % app->use_char_count];
			}
			app->encode[i] = '\0';

			free(t);
			rand48_destroy(r);
		}
	}

	return app->encode;
}
