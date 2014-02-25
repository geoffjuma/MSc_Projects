#!/usr/bin/env bash

echo "enter your total marks:"
read marks;

if (($marks -ge 40)&&($marks -lt 50)) then
{
echo "Diel dog dala"
}
elif (($marks -ge 50 )&&($marks -lt 60 )) then
{
echo "You have a pass"
}
elif (($marks -ge 60 )&&($marks -lt 70 )) then
{
echo "You have a C"
}
else
echo "You have a B"
fi
