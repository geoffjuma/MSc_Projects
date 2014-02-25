<?php
	
require_once('../db/db_schedule.php');
require_once('../ent/timeout.php');


if($_SERVER['REQUEST_METHOD'] == 'GET'){
	//Фильтруем полученые данные
	$log = mysql_real_escape_string($_GET['log']);
	$pwd = mysql_real_escape_string($_GET['pwd']);
	
	$datetime = time();
	
	//получаем токен
	$url = "http://artemgorev.ru/api/v1/signin?login=$log&password=$pwd";
	$json = file_get_contents($url);
	$obj = json_decode($json, true);
	$token1 = $obj["token"];
	//echo $token1.'<br>';

	//получаем id-пользователя
	$url = "http://artemgorev.ru/api/v1/users/?login=$log&format=json&token=$token1";
	$json = file_get_contents($url);
	$obj = json_decode($json, true);
	$id = $obj["objects"]["0"]["id"];

	//проверяем нет ли в таблице данного пользователя
	$sql1 = "SELECT * FROM UserSession where user_id='$id'";
	$res=mysql_query($sql1) or die(mysql_error());
	if(($num_rows =  mysql_num_rows($res)) == 0) {
		//если нет - добавляем его
		//$sql1 = "INSERT into UserSession (user_id, last_time, token) values ('$id', '$datetime', '$token1');";
		$sql1 = "INSERT into UserSession (user_id, last_time, token) values ('$id',  NOW(), '$token1');";		
mysql_query($sql1) or die(mysql_error());
//$sql2 = "DELETE FROM UserSession 
//              WHERE last_time <= NOW() - INTERVAL 1 SECOND";
//$res=mysql_query($sql2) or die(mysql_error());
	}
	else {
		//если есть - заменяем токен на новый
		$sql1 = "UPDATE UserSession SET token='$token1' WHERE user_id='$id';";
		mysql_query($sql1) or die(mysql_error());
//$sql2 = "DELETE FROM UserSession 
//              WHERE last_time <= NOW() - INTERVAL 1 SECOND";
//$res=mysql_query($sql2) or die(mysql_error());
	}
	
	//ВАЖНО!!! Заголовки функцией header нужно отправлять до любого вывода текста в браузер! Даже перед <?php не должно быть пробелов!
	//header('Location: ../ent/all_schedule.php?token='.$token1); //делает редирект на указанный адрес.
	
	//выводим сгенерированный токен для данного пользователя
	$sql1 = "SELECT * FROM UserSession where user_id='$id'";
	$res=mysql_query($sql1) or die(mysql_error());
	while($row1 = mysql_fetch_assoc($res)){
		$json_data = array ('token'=>$row1['token']);
		echo json_encode($json_data);
	}

	mysql_close();
}
?>

<html> 
<head></head>
<title></title>
<body>
<center> <h3> Welcome! </h3> <br> </center>

<form method="POST" action="<?=$_SERVER['PHP_SELF']?>">
<table align="center" width="30%">
   <tr>
      <td>Login: </td>
      <td><input type="text" name="usr" size="11" maxlength="10"></td>
   </tr>
   <tr>
      <td>Password: </td>
      <td><input type="text" name="pwd" size="11" maxlength="10"></td>
   </tr>
   <tr>
      <td><input type="reset"></td>
      <td><input type="submit" value="Enter"></td>
   </tr>
</table>
</form>

</body>
</html>

