/* passgen extension for PHP */

#ifndef PHP_PASSGEN_H
# define PHP_PASSGEN_H

extern zend_module_entry passgen_module_entry;
# define phpext_passgen_ptr &passgen_module_entry

# define PHP_PASSGEN_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_PASSGEN)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_PASSGEN_H */
