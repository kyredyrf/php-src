/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
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

/* If you declare any globals in php_passgen.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(passgen)
*/

/* True global resources - no need for thread safety here */
static int le_passgen;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("passgen.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_passgen_globals, passgen_globals)
    STD_PHP_INI_ENTRY("passgen.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_passgen_globals, passgen_globals)
PHP_INI_END()
*/
/* }}} */

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

/* {{{ php_passgen_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_passgen_init_globals(zend_passgen_globals *passgen_globals)
{
	passgen_globals->global_value = 0;
	passgen_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(passgen)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(passgen)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(passgen)
{
#if defined(COMPILE_DL_PASSGEN) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(passgen)
{
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

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
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
 *
 * Every user visible function must have an entry in passgen_functions[].
 */
const zend_function_entry passgen_functions[] = {
	PHP_FE(passgen,		arginfo_passgen)
	PHP_FE(passgen_v2,	arginfo_passgen_v2)
	PHP_FE_END	/* Must be the last line in passgen_functions[] */
};
/* }}} */

/* {{{ passgen_module_entry
 */
zend_module_entry passgen_module_entry = {
	STANDARD_MODULE_HEADER,
	"passgen",
	passgen_functions,
	PHP_MINIT(passgen),
	PHP_MSHUTDOWN(passgen),
	PHP_RINIT(passgen),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(passgen),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(passgen),
	PHP_PASSGEN_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PASSGEN
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(passgen)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
