<html>

<head>
</head>
<title></title>
        <style type="text/css"> 
            body 
            { 
                margin: 0; 
                padding: 0; 
            }

            .common 
            { 
                border: 1px solid black; 
            }
	#centered 
		{
    		position:           absolute;
	    	display:            inline-block;
    		left:           -500px;
    		width:          1000px;
    		margin:             0 50%;
		}


            #middlecontainer
            {
                float: left;
                width: 104px;
                margin: 0 549px;
            }

            #container { 
                float: left;
            } 

            #Rightborder { 
		height:350px;
		width:100px;
		float:left;
                background:#FFA500;  
                float:left;
            }
	#Leftborder { 
		height:350px;
		width:100px;
		float:left;
                background:#FFA500;  
                float:right;
            }

            #footer { 
                background: #75A1D0; 
		text-align:center;
		clear:both;
		color:black;
            }

	#input{
		
		width: 15em;
    		width: 15em;
		border:2px solid #dadada;
    		border-radius:7px;
    		font-size:20px;
    		padding:5px; 
		color:blue;
		}


	#input [type=text]{
		border:2px solid #dadada;
    		border-radius:7px;
    		font-size:20px;
    		padding:5px; 
		width: 17em;
    		width: 17em;
		float:right;
	}
       #input:focus { 
     		outline:none;
    		border-color:#9ecaed;
    		box-shadow:0 0 10px #9ecaed;
	}
	#menu {
  	position: fixed;
  	right: 0;
  	top: 50%;
  	width: 8em;
  	margin-top: -2.5em;
	}
 	
</style>

<?php 
//start session
//start session
session_start();
if (!isset($_SESSION['count'])) 
{
  $_SESSION['counter'] = 0;
} else 
{
  $_SESSION['counter']++;
}

//connect to the db

include 'db_connect.php';

	if (isset($_POST['login']))
		{	
	
			$token=$_POST['token'];

			//protect SQL Injection
	
			$token = stripslashes(token);
			$token = mysql_real_escape_string($token);
	
			//Query
			$sql = "SELECT * FROM Sessions WHERE Token='$token'" ;
			$result=mysql_query($sql);
				if (!$result)
					{
						die ("SQL Querry Error! Check your code!". mysql_error());
					}

			// Mysql_num_row is counting table row
			$count=mysql_num_rows($result);

	// If result matched $myusername and $mypassword, table row must be 1 row
	if($count==1)
 	{

	// Register $myusername, $mypassword and redirect to file "login_success.php"
	echo '<center>','welcome Mr','</center>';
	//session_register("token"); 
	//header("location:chatApp.php");
	}
	else 
	{
	echo '<center>','<font color= "red">', "check your token again",'</font>','</center>';	
	}
}
?>



<body>
<center>
<div id="centered">

<div id="footer">
<h2 style="color:white">
<a href="#" style="text-decoration:none;color:white;">Welcome <?php echo $username; ?></a>to ChatApp!</h2>
</div>


	<div id="middleware">
	
	  
	
		<table><tr><td><img src="images/logo.jpg"> </td></tr></table> 
		<table><form name="loginForm" action = "login.php" method = "POST" onSubmit= "return validateFormOnSubmit(this)" >
		
		
		<p><tr><td style="color:blue;"><b>Token<b>  </td><td><input type="password" name="token" id= "input"><br></td></td></p>
			   <table><tr><td><input type="Submit" value="logIn" name="login" id ="input"> 
			   <input type="reset" value="Clear" name="register" id = "input"></td></tr>
			   <tr><td><a href="signUp.php" style="text-decoration:none;color:red;">Not Registered? </a></td></tr>
		
			   </table>
		</form>
		</table>
<image></image>
	</div>


<div id="footer"">
<h3 style="color:white"> @geoffjuma</h3></div>

</div>
</body>
</center>
</html>
