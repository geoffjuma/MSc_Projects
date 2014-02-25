<?php
//connect to the database
include 'db_connect.php';


//call the pass function

if (isset($_GET['method']) && !empty($_GET['method']))
{
 if (function_exists ($_GET['method']))
	{
		$_GET['method']();
	}
}


//method for login credentions

//method for querrying database


function getCredentials()
{
	$token = '7d54a34e77bec8c8711af5a0e53419bf';
	//$username = mysql_real_escape_string($username);
        $query = "SELECT Token FROM Sessions WHERE Token = '$token';";

	//check for database errors;

	if (!query)
		 {
     		die('something is wrong with your query formation: ' . mysql_error());
     		}
	        $result = mysql_query($query);
		//echo $result;
 
		if(mysql_num_rows($result) == 0) // User not found. So, redirect to login_form again.
			{
			echo ("Your token is not correct");
    			header('Location: login.html');
			}
	
	$users = array();
	while ($user = mysql_fetch_array($result))
	{
		$users[] = $user;
	}
	$users = json_encode($users);

	echo $_GET['jsoncallback'] .'(' . $users . ')';

}


//method for querrying database for users


function getAllUsers()
{
	$user_sql = mysql_query("SELECT * FROM User");
	
	$users = array();
	while ($user = mysql_fetch_array($user_sql))
	{
		$users[] = $user;
	}
	$users = json_encode($users);

	echo $_GET['jsoncallback'] .'(' . $users . ')';
}

?>

