; homeall.g
; called to home all axes
;
; generated by RepRapFirmware Configuration Tool v2.1.3 on Sun Dec 01 2019 12:21:58 GMT-0500 (Eastern Standard Time)
G91                     ; relative positioning
G1 H2 Z5 F6000          ; lift Z relative to current position
G1 H1 X-305 Y308 F1800  ; move quickly to X and Y axis endstops and stop there (first pass)
G1 H2 X5 Y-5 F6000      ; go back a few mm
G1 H1 X-305 Y308 F360   ; move slowly to X and Y axis endstops once more (second pass)
G90
G1 X-20 Y255 F6000
G91
G1 H1 Z-275 F1800       ; move Z down until the endstop is triggered
G92 Z-0.925             ; set Z position to endstop height
G1 H2 Z5 F100           ; lift Z relative to current position

