REM echo all args %*
if %2 == "SG"	goto sg
if %2 == "SMS" 	goto SMS
if %2 == "SC"	goto SC

echo other platform
%1
goto exit

:SMS
%1.sms
goto exit

:SG
%1.sg
goto exit

:SC
%1.sc
goto exit

:exit
