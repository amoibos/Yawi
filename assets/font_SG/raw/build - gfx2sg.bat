
python ../../tools/gfx2sg.py font.png --transparent=1 --compress %* 
copy /y "font (palette).bin" "../font (palette).bin"
copy /y "font (tiles).bin" "../font (tiles).bin"
REM pause
