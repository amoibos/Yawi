
python ../../../tools/gfx2sg.py font.png --transparent=1 --compress %* 
move /y "font (palette).bin" "../font (palette).bin"
move /y "font (tiles).bin" "../font (tiles).bin"
REM pause
