mkts.exe platform=sg1000 in=font.png out=bin mode=chars label=pd_font size=16,16 max=256 silent
copy /y pd_font_cgt.bin "../font (palette).bin"
copy /y pd_font_pgt.bin "../font (tiles).bin"
pause