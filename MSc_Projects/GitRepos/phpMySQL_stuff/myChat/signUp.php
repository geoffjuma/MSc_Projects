<html>

<head> </head>
<title>Chat App!
</title>
        <style type="text/css"> 
            #body 
            { 
                margin: 0; 
                padding: 0; 
            }

            #common 
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
</style>

<?php 

//connect to the db

require'db_connect.php';
require 'token_gen.php';

//declare the variables
 
     $username = $_POST['username'];
     $email = $_POST['email'];
     $password = $_POST['password'];
     $token = $key;
//check if submit is clicked

	if (isset($_POST['signUp']))

	{
		if (!get_magic_quotes_gpc()) 
		{
	
	 		$_POST['username'] = addslashes($_POST['username']);
	
	 	}

		 $usercheck = $_POST['username'];

		 $check = mysql_query("SELECT username FROM User WHERE username = '$usercheck'") 

		or die(mysql_error());

		 $check2 = mysql_num_rows($check);

		 //if the name exists it gives an error

		 if ($check2 != 0) 
	{

	 		echo '<center>','Sorry, the username '.$_POST['username'].' is already in use.','</center>';

    		
	}
		else
	{

    	$sql_insertUser = "INSERT INTO User (Username, Email, Password) VALUES ('$username', '$email', '$password')";
	$sql_insertSession = "INSERT INTO Sessions (Token) VALUES ('$token')";
	$retval_user = mysql_query( $sql_insertUser);
	$retval_session = mysql_query( $sql_insertSession);
	
		if(!$retval_user && !retval_session )
		{
 	 		die('Could not enter data: ' . mysql_error());
		}
		echo '<center>',"Successfully registered",'<a href="login.php"> click here, </a>','to login', '</center>','<br>';
		echo '<center>',"your token is ". $token.'</center>';
		mysql_close($link);	

	}
}

?>

<script type="text/javascript">

/* the main validation function */

function validateFormOnSubmit(signupForm)
{
	var reason ="";
	reason +=validateUsername(signupForm.username);
	reason +=validatePassword(signupForm.password);
	reason +=validateEmail(signupForm.email);
	if (reason != "") 
	{
	    alert("Enter all the fields correctly:\n" + reason);
	    return false;
	} 
	else
  return true;
}

/*usename validation function */
function validateUsername(fld)
 	{
	var error = "";
	var illegalChar =/\W/; /* allows letters, numbers and underscore*/
	if (fld.value =="")
		{
		fld.style.background="yellow";
		error="empty Username!\n";
		} 
	else if (fld.value.length<5 && fld.value.length>10)
	{
		fld.style.background="Yellow";
		error="you entered a lot of crap :)\n;"
	}
	 else 
	{
	fld.style.background="white";
	} 
	return error;
 }

function trim(s)
	{
  	return s.replace(/^\s+|\s+$/, '');
	} 

function validateEmail(fld) {
    var error="";
    var tfld = trim(fld.value);                        // value of field with whitespace trimmed off
    var emailFilter = /^[^@]+@[^@.]+\.[^@]*\w\w$/ ;
    var illegalChars= /[\(\)\<\>\,\;\:\\\"\[\]]/ ;
    
    if (fld.value == "")
	 {
        fld.style.background = 'Yellow';
        error = "You didn't enter an email address.\n";
    } else if (!emailFilter.test(tfld)) {              //test email for illegal characters
        fld.style.background = 'Yellow';
        error = "Please enter a valid email address.\n";
    } else if (fld.value.match(illegalChars)) {
        fld.style.background = 'Yellow';
        error = "The email address contains illegal characters.\n";
    } else {
        fld.style.background = 'White';
    }
    return error;
}	

//password validation function
function validatePassword(fld)
 {
	var error = "";
	var illegalChar =/\W/; //allows letters, numbers and underscore
	if (fld.value=="")
		{
		fld.style.background="yellow";
		error="empty Password!\n";
	} 
	else if (fld.value.length<5 && fld.value.length>10)
	{
	fld.style.background="Yellow";
	error="you entered a lot of crap :)\n;"
	}
	 else if (!((fld.value.search(/(a-z)+/)) && (fld.value.search(/(0-9)+/)))) {
        error = "The password must contain at least one numeral.\n";
        fld.style.background = 'Yellow';
	}
	 else
	{
	fld.style.background="white";
	} 
	return error;
	}


</script> 	
        
<body>
<center>
<div id="centered">

<div id="footer">
<h2 style="color:white">ChatApp!</h2>
</div>


	<div id="middleware">
		<table><tr><td><img src="images/logo.jpg"> </td></tr></table> 
		<table><form name= "signupForm" action="signUp.php" method="POST" onSubmit= "return validateFormOnSubmit(this);">
		<p><tr><td style="color:blue;"><b>Username</b>  </td><td><input type="text" name="username"  id ="input"><br></td></tr></p>
		<p><tr><td style="color:blue;"><b>E-mail	</b></td><td>  <input type="text" name="email" id ="input"><br></td></tr></p>
		<p><tr><td style="color:blue;"><b>Password<b>  </td><td><input type="password" name="password" id ="input"><br></td></td></p>
			   <table><tr><td><b><input type="submit" value="SignUp" name="signUp" id ="input" ></b> 
			   <input type="reset" value="Clear" name="clear" id="input"></td></tr>
			   </table>
		</form>
		</table>
<image></image>
	</div>

<br>

<div id="footer"">
<h3 style="color:white"> copyright! @geoffjuma </h3>
</div>

</div>
</body>
</center>
</html>
