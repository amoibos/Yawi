python ../../tools/gfx2sg.py city.png --compress
python ../../tools/gfx2sg.py cemetry.png --compress

copy /y "city (palette).bin" "../city (palette).bin"
copy /y "city (tiles).bin" "../city (tiles).bin"

copy /y "cemetry (palette).bin" "../cemetry (palette).bin"
copy /y "cemetry (tiles).bin" "../cemetry (tiles).bin"
pause
