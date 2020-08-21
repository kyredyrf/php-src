--TEST--
Check if passgen is loaded
--SKIPIF--
<?php
if (!extension_loaded('passgen')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "passgen" is available';
?>
--EXPECT--
The extension "passgen" is available
