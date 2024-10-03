mkts.exe platform=sg1000 in=city.png out=bin mode=chars label=city size=16,16 max=256 silent
copy /y city_cgt.bin "../city (palette).bin"
copy /y city_pgt.bin "../city (tiles).bin"
pause