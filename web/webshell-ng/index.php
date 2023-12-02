<?php
$memory = $_REQUEST;
if (count($memory) == 0) {
    highlight_file(__FILE__);
    die();
}
$memory["pc"] = 0;
$i = &$memory["pc"];
while ($i < 1024) {
    if ($memory[$i] === "call") {
        $memory[$memory[$i + 3]] = call_user_func($memory[$i + 1], $memory[$memory[$i + 2]]);
        $i += 4;
    } else if ($memory[$i] === "inst_call") {
        $inst = $memory[$memory[$i + 1]];
        $method = $memory[$i + 2];
        $memory[$memory[$i + 4]] = call_user_func([$inst, $method], $memory[$memory[$i + 3]]);
        $i += 5;
    } else if ($memory[$i] === "new") {
        $class_name = $memory[$i + 1];
        $memory[$memory[$i + 3]] = new $class_name($memory[$memory[$i + 2]]);
        $i += 4;
    } else {
        die();
    }
}