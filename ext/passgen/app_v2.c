/**
 *	アプリ
 */

#include "app.h"

#include "hash_value.h"
#include "rand48.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_COUNT 10
#define ALPHABET_COUNT 26
#define SYMBOLIC_COUNT ((126 - 32 + 1) - (NUMBER_COUNT + ALPHABET_COUNT * 2))

typedef enum UseType
{
	UseType_Number,
	UseType_Lower,
	UseType_Upper,
	UseType_Symbolic,
	UseType_Count
}
UseType;

struct app_v2_t 
{
	char use_number[NUMBER_COUNT + 1];
	int use_number_count;
	char use_lower[ALPHABET_COUNT + 1];
	int use_lower_count;
	char use_upper[ALPHABET_COUNT + 1];
	int use_upper_count;
	char use_symbolic[SYMBOLIC_COUNT + 1];
	int use_symbolic_count;
	char* keyword;
	int encode_count;

	char* encode;
};

app_v2_t* app_v2_create()
{
	app_v2_t* app;

	app = (app_v2_t*)malloc(sizeof(*app));
	app->use_number[0] = '\0';
	app->use_number_count = 0;
	app->use_lower[0] = '\0';
	app->use_lower_count = 0;
	app->use_upper[0] = '\0';
	app->use_upper_count = 0;
	app->use_symbolic[0] = '\0';
	app->use_symbolic_count = 0;
	app->keyword = NULL;
	app->encode_count = 0;
	app->encode = NULL;

	return app;
}

void app_v2_destroy(app_v2_t* app)
{
	if (app != NULL)
	{
		free(app->encode);
		free(app);
	}
}

void app_v2_parse_arguments(app_v2_t* app, int argc, const char* argv[])
{
	int i;

	for (i = 0; i < argc; i++)
	{
		if (argv[i][0] == '-' || argv[i][0] == '/')
		{
			if (argv[i][1] == 'n' || strcmp(argv[i], "--use-number") == 0)
			{
				app_v2_set_use_number(app, argv[++i]);
			}
			else if (argv[i][1] == 'l' || strcmp(argv[i], "--use-lower") == 0)
			{
				app_v2_set_use_lower(app, argv[++i]);
			}
			else if (argv[i][1] == 'u' || strcmp(argv[i], "--use-upper") == 0)
			{
				app_v2_set_use_upper(app, argv[++i]);
			}
			else if (argv[i][1] == 's' || strcmp(argv[i], "--use-symbolic") == 0)
			{
				app_v2_set_use_symbolic(app, argv[++i]);
			}
			else if (argv[i][1] == 'k' || strcmp(argv[i], "--keyword") == 0)
			{
				app_v2_set_keyword(app, argv[++i]);
			}
			else if (argv[i][1] == 'c' || strcmp(argv[i], "--count") == 0)
			{
				app_v2_set_encode_count(app, atoi(argv[++i]));
			}
		}
	}
}

static int app_v2_is_number(int c) { return isdigit(c); }
static int app_v2_is_lower(int c) { return islower(c); }
static int app_v2_is_upper(int c) { return isupper(c); }
static int app_v2_is_symbolic(int c) { return isprint(c); }

static void app_v2_set_use_char(app_v2_t* app, const char* str, UseType type)
{
	char* app_use_char;
	int* app_use_char_count;
	int (*app_is_func)(int);
	int i;
	char map[128];
	int length;
	int c;
	char use_char[128];

	switch (type)
	{
		case UseType_Number:
			app_use_char = app->use_number;
			app_use_char_count = &app->use_number_count;
			app_is_func = app_v2_is_number;
			break;
		case UseType_Lower:
			app_use_char = app->use_lower;
			app_use_char_count = &app->use_lower_count;
			app_is_func = app_v2_is_lower;
			break;
		case UseType_Upper:
			app_use_char = app->use_upper;
			app_use_char_count = &app->use_upper_count;
			app_is_func = app_v2_is_upper;
			break;
		case UseType_Symbolic:
			app_use_char = app->use_symbolic;
			app_use_char_count = &app->use_symbolic_count;
			app_is_func = app_v2_is_symbolic;
			break;
	}

	free(app->encode);
	app->encode = NULL;

	for (i = 0; i < (int)(sizeof(map) / sizeof(map[0])); i++)
		map[i] = 0;

	length = strlen(str);
	for (i = 0; i < length; i++)
	{
		c = (int)str[i];
		if (app_is_func(c))
			map[c] = 1;
	}

	*app_use_char_count = 0;
	for (i = 0; i < (int)(sizeof(map) / sizeof(map[0])); i++)
	{
		if (map[i] != 0)
			use_char[(*app_use_char_count)++] = i;
	}
	use_char[*app_use_char_count] = '\0';

	strcpy(app_use_char, use_char);
}

void app_v2_set_use_number(app_v2_t* app, const char* str)
{
	app_v2_set_use_char(app, str, 0);
}
void app_v2_set_use_lower(app_v2_t* app, const char* str)
{
	app_v2_set_use_char(app, str, 1);
}
void app_v2_set_use_upper(app_v2_t* app, const char* str)
{
	app_v2_set_use_char(app, str, 2);
}
void app_v2_set_use_symbolic(app_v2_t* app, const char* str)
{
	app_v2_set_use_char(app, str, 3);
}

void app_v2_set_keyword(app_v2_t* app, const char* str)
{
	free(app->keyword);
	free(app->encode);
	app->encode = NULL;

	app->keyword = (char*)malloc(strlen(str) + 1);
	strcpy(app->keyword, str);
}

void app_v2_set_encode_count(app_v2_t* app, int count)
{
	free(app->encode);
	app->encode = NULL;

	app->encode_count = count;
}

const char* app_v2_encode(app_v2_t* app)
{
	if (app->encode == NULL)
	{
		if ((app->use_number_count + app->use_lower_count + app->use_upper_count + app->use_symbolic_count <= 0) || app->keyword == NULL || app->encode_count <= 0)
		{
			app->encode = (char*)malloc(1);
			app->encode[0] = '\0';
		}
		else
		{
			rand48_t* r;
			char encode_count_text[12];
			char* hash_source;
			UseType type_order[(int)UseType_Count];
			UseType* types;
			UseType type;
			int type_decided;
			int type_index;
			int i;
			int a, b;
			char* app_use_char;
			int app_use_char_count;

			r = rand48_create();

			sprintf(encode_count_text, "%d", app->encode_count);
			hash_source = (char*)malloc(app->use_number_count + app->use_lower_count + app->use_upper_count + app->use_symbolic_count + strlen(app->keyword) + strlen(encode_count_text));
			strcpy(hash_source, app->use_number);
			strcat(hash_source, app->use_lower);
			strcat(hash_source, app->use_upper);
			strcat(hash_source, app->use_symbolic);
			strcat(hash_source, app->keyword);
			strcat(hash_source, encode_count_text);
			rand48_set_seed(r, get_hash_value(hash_source));
			free(hash_source);
			hash_source = NULL;

			for (i = 0; i < (int)UseType_Count; i++)
				type_order[i] = (UseType)i;
			for (i = 0; i < (int)UseType_Count * (int)UseType_Count; i++)
			{
				a = rand48_get_nonnegative_integer(r) % (int)UseType_Count;
				b = rand48_get_nonnegative_integer(r) % (int)UseType_Count;
				type = type_order[a];
				type_order[a] = type_order[b];
				type_order[b] = type;
			}

			types = (UseType*)malloc(app->encode_count * sizeof(UseType));
			type_index = 0;
			type = type_order[type_index];
			for (i = 0; i < app->encode_count; i++)
			{
				type_decided = 0;
				do
				{
					switch (type)
					{
						case UseType_Number:
							if (app->use_number_count > 0)
								type_decided = 1;
							break;
						case UseType_Lower:
							if (app->use_lower_count > 0)
								type_decided = 1;
							break;
						case UseType_Upper:
							if (app->use_upper_count > 0)
								type_decided = 1;
							break;
						case UseType_Symbolic:
							if (app->use_symbolic_count > 0)
								type_decided = 1;
							break;
					}
					if (type_decided == 0)
					{
						type_index = (type_index + 1) % UseType_Count;
						type = type_order[type_index];
					}
				}
				while (type_decided == 0);

				types[i] = type;
				type_index = (type_index + 1) % UseType_Count;
				type = type_order[type_index];
			}
			for (i = 0; i < app->encode_count * app->encode_count; i++)
			{
				a = rand48_get_nonnegative_integer(r) % app->encode_count;
				b = rand48_get_nonnegative_integer(r) % app->encode_count;
				type = types[a];
				types[a] = types[b];
				types[b] = type;
			}

			app->encode = (char*)malloc(app->encode_count + 1);
			for (i = 0; i < app->encode_count; i++)
			{
				switch (types[i])
				{
					case UseType_Number:
						app_use_char = app->use_number;
						app_use_char_count = app->use_number_count;
						break;
					case UseType_Lower:
						app_use_char = app->use_lower;
						app_use_char_count = app->use_lower_count;
						break;
					case UseType_Upper:
						app_use_char = app->use_upper;
						app_use_char_count = app->use_upper_count;
						break;
					case UseType_Symbolic:
						app_use_char = app->use_symbolic;
						app_use_char_count = app->use_symbolic_count;
						break;
				}
				app->encode[i] = app_use_char[rand48_get_nonnegative_integer(r) % app_use_char_count];
			}
			app->encode[i] = '\0';

			rand48_destroy(r);
		}
	}

	return app->encode;
}
