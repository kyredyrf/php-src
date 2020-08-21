/* passgen extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_passgen.h"

#include "hash_value.h"
#include "hash_value.c"
#include "rand48.h"
#include "rand48.c"
#include "app.h"
#include "app.c"
#include "app_v2.h"
#include "app_v2.c"
#include <stdlib.h>

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

/* {{{ void passgen_test1()
 */
PHP_FUNCTION(passgen_test1)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_printf("The extension %s is loaded and working!\r\n", "passgen");
}
/* }}} */

/* {{{ string passgen_test2( [ string $var ] )
 */
PHP_FUNCTION(passgen_test2)
{
	char *var = "World";
	size_t var_len = sizeof("World") - 1;
	zend_string *retval;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(var, var_len)
	ZEND_PARSE_PARAMETERS_END();

	/* retval = strpprintf(0, "Hello %s", var); */
	retval = NULL;

	RETURN_STR(retval);
}
/* }}}*/

/* {{{ string passgen( [ string $use_char, string $keyword, string $encode_count ] )
 */
PHP_FUNCTION(passgen)
{
	zend_string *use_char;
	zend_string *keyword;
	zend_string *encode_count;
	zend_string *retval;
	app_t* app;
	const char* encode;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_STR(use_char)
		Z_PARAM_STR(keyword)
		Z_PARAM_STR(encode_count)
	ZEND_PARSE_PARAMETERS_END();

	app = app_create();
	app_set_use_char(app, ZSTR_VAL(use_char));
	app_set_keyword(app, ZSTR_VAL(keyword));
	app_set_encode_count(app, atoi(ZSTR_VAL(encode_count)));
	encode = app_encode(app);
	retval = zend_string_init(encode, strlen(encode), 0);
	app_destroy(app);

	RETURN_STR(retval);
}
/* }}}*/

/* {{{ string passgen_v2( [ string $use_number, string $use_lower, string $use_upper, string $use_symbolic, string $keyword, string $encode_count ] )
 */
PHP_FUNCTION(passgen_v2)
{
	zend_string *use_number;
	zend_string *use_lower;
	zend_string *use_upper;
	zend_string *use_symbolic;
	zend_string *keyword;
	zend_string *encode_count;
	zend_string *retval;
	app_v2_t* app;
	const char* encode;

	ZEND_PARSE_PARAMETERS_START(6, 6)
		Z_PARAM_STR(use_number)
		Z_PARAM_STR(use_lower)
		Z_PARAM_STR(use_upper)
		Z_PARAM_STR(use_symbolic)
		Z_PARAM_STR(keyword)
		Z_PARAM_STR(encode_count)
	ZEND_PARSE_PARAMETERS_END();

	app = app_v2_create();
	app_v2_set_use_number(app, ZSTR_VAL(use_number));
	app_v2_set_use_lower(app, ZSTR_VAL(use_lower));
	app_v2_set_use_upper(app, ZSTR_VAL(use_upper));
	app_v2_set_use_symbolic(app, ZSTR_VAL(use_symbolic));
	app_v2_set_keyword(app, ZSTR_VAL(keyword));
	app_v2_set_encode_count(app, atoi(ZSTR_VAL(encode_count)));
	encode = app_v2_encode(app);
	retval = zend_string_init(encode, strlen(encode), 0);
	app_v2_destroy(app);

	RETURN_STR(retval);
}
/* }}}*/

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(passgen)
{
#if defined(ZTS) && defined(COMPILE_DL_PASSGEN)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(passgen)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "passgen support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_passgen_test1, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_passgen_test2, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_passgen, 0)
	ZEND_ARG_INFO(0, use_char)
	ZEND_ARG_INFO(0, keyword)
	ZEND_ARG_INFO(0, encode_count)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_passgen_v2, 0)
	ZEND_ARG_INFO(0, use_number)
	ZEND_ARG_INFO(0, use_lower)
	ZEND_ARG_INFO(0, use_upper)
	ZEND_ARG_INFO(0, use_symbolic)
	ZEND_ARG_INFO(0, keyword)
	ZEND_ARG_INFO(0, encode_count)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ passgen_functions[]
 */
static const zend_function_entry passgen_functions[] = {
	PHP_FE(passgen,		arginfo_passgen)
	PHP_FE(passgen_v2,	arginfo_passgen_v2)
	PHP_FE_END
};
/* }}} */

/* {{{ passgen_module_entry
 */
zend_module_entry passgen_module_entry = {
	STANDARD_MODULE_HEADER,
	"passgen",					/* Extension name */
	passgen_functions,			/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(passgen),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(passgen),			/* PHP_MINFO - Module info */
	PHP_PASSGEN_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PASSGEN
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(passgen)
#endif

