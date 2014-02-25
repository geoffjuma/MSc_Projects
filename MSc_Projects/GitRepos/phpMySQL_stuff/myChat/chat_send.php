<?php
require_once('db_connect.php');

//db_connect();

$msg = $_GET["msg"];
//$dt = date("Y-m-d H:i:s");
$user = $_GET["name"];

$sql="INSERT INTO Message(Username, Message) values($user,$msg)";

     // echo $sql;
$result = mysql_query($sql);
if(!$result)
{
    throw new Exception('Query failed: ' . mysql_error());
    exit();
}
?>
