echo params %0 %1 %2
if %2 == "SG"	goto sg
if %2 == "SMS" 	goto SMS

echo other platform
%1
goto exit

:SMS
%1.sms
goto exit

:SG
%1.sg
goto exit

:exit