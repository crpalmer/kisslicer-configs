; resume.g
; called before a print from SD card is resumed
;
; generated by RepRapFirmware Configuration Tool v2 on Fri Jan 04 2019 20:46:37 GMT-0500 (Eastern Standard Time)
G1 R1 X0 Y0 Z5 F6000 ; go to 5mm above position of the last print move
G1 R1 X0 Y0          ; go back to the last print move
M83                  ; relative extruder moves
G1 E1 F3600         ; extrude 1mm of filament

