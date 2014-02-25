#! /usr/bin/env bash
echo "Kindly enter your two names:";
read name1 name2;
echo "now enter your full adress:";
read address street city;
echo "Finally enter your phone number";
read phone;
echo "Name: "$name1, $name2 > details.out
echo "Adress: "$address, $street, $city >> details.out
echo "Phone: "$phone >> details.out
echo "Your details are contained in \"details.out"\"
