<html>
<title></title>
<head></head>
<body>
<?php 

function default_values_test ($a = 123, $b = 456){ 
   echo "a = ".$a."<br/>"; 
   echo "b = ".$b."<br/>"; 
   echo "<br/>"; 
} 

default_values_test(); // uses values set in 'header' 
default_values_test('overwritten', 987654321); // uses these values 
default_values_test($non_existent, "var A has to be overwritten."); // you can only use this for the last vars. 

?> 

</body>
</html>
