<html>
	<head>
	</head>
	<body>

<?php 
include "CSS/main.css";
?>

<script style=text/java>
//the main validation function
function validateFormOnSubmit(login){
var reason ="";
reason +=validateUsername(login.username);
reason +=validatePassword(login.pwd);
if (reason != "") {
    alert("Enter all the fields correctly:\n" + reason);
    return false;
  }

  return true;
}

//usename validation function
function validateUsername(fld)
 {
var error = "";
var illegalChar =/\W/; //allows letters, numbers and underscore
if (fld.value=="")
{
	fld.style.background="yellow";
	error="empty Username!\n";
} else if (fld.value.length<5 && fld.value.length>10)
{
	fld.style.background="Yellow";
	error="you entered a lot of crap :)\n;"
} else 
{
fld.style.background="white";
} return error;
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
} else if (fld.value.length<5 && fld.value.length>10)
{
	fld.style.background="Yellow";
	error="you entered a lot of crap :)\n;"
} else if (!((fld.value.search(/(a-z)+/)) && (fld.value.search(/(0-9)+/)))) {
        error = "The password must contain at least one numeral.\n";
        fld.style.background = 'Yellow';
} else
{
fld.style.background="white";
} return error;
}
 

</script>


	<table bgcolor="green" border ="0" color="black" width="80%" height=100% align="center">
	<tr><td><table bgcolor="white" border ="0" color="black" width="70%" height=90% align="center">
	<tr><td> 
		<img src="My_doctor.jpg" alt="image not found" style="width:100%;height:30%;" align ="top"> <br><br><br>
		<br><br><br><br>
		
		<form align="center" name="login" action="my_doctor_login.php" method="POST" onsubmit="return validateFormOnSubmit(this)">
		  <table align="center"><tr><td> 
			<font color="red"><b>* Username: </b></font><input type="text" name="username" maxlength="20" style="height:25;width:200;border-color:blue;"><br><br>
                          </td></tr> 
			<tr><td>
			<font color="red"><b>* Password: </b></font><input type="password" name="pwd" maxlength ="20" style="height:25;width:200;border-color:blue;"> <font color="blue"><a href="forgot.php" style="text-decoration:none;">Forgot?</a></font> <br><br>
			</td></tr><tr><td>
			<input type ="submit" name="login" value="Login" style="height:25;width:290;border-color:red">
			</td></tr>
			</table>
		</form>
	</td></tr> 
	</table></td></tr>
	</table>

	
</html>
