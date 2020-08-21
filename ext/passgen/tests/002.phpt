--TEST--
test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('passgen')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = test1();

var_dump($ret);
?>
--EXPECT--
The extension passgen is loaded and working!
NULL
