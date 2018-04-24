; Configuration file for Duet WiFi (firmware version 1.20 or newer)
; executed by the firmware on start-up
;
; generated by RepRapFirmware Configuration Tool on Sun Apr 01 2018 06:51:06 GMT-0400 (Eastern Daylight Time)

; General preferences
G90                                       ; Send absolute coordinates...
M83                                       ; ...but relative extruder moves
M665 R157 L398 B170 H520                  ; Set delta radius, diagonal rod length, printable radius and homed height
M666 X0 Y0 Z0                             ; Put your endstop adjustments here, or let auto calibration find them

; Network
; Network
M550 Ptlm-duet                            ; Set machine name
M552 S1                                   ; Enable network
M98 P/sys/passwords.g                     ; M587 S"..." P"..." & M551 P...
M552 S1                                   ; Enable network
M586 P0 S1                                ; Enable HTTP
M586 P1 S0                                ; Disable FTP
M586 P2 S0                                ; Disable Telnet

; Drives
M569 P0 S1                                ; Drive 0 goes forwards
M569 P1 S1                                ; Drive 1 goes forwards
M569 P2 S1                                ; Drive 2 goes forwards
M569 P3 S1                                ; Drive 3 goes forwards
M569 P4 S1                                ; Drive 4 goes forwards
M350 X32 Y32 Z32 E16:16 I1                ; Configure microstepping with interpolation
M92 X160 Y160 Z160 E810:5400              ; Set steps per mm
M566 X1200 Y1200 Z1200 E300:300           ; Set maximum instantaneous speed changes (mm/min)
M203 X39960 Y39960 Z39960 E1200:1200      ; Set maximum speeds (mm/min)
M201 X5000 Y5000 Z5000 E500:120           ; Set accelerations (mm/s^2)
M906 X1000 Y1000 Z1000 E550:500 I30       ; Set motor currents (mA) and motor idle factor in per cent
M84 S30                                   ; Set idle timeout

; Axis Limits
M208 Z0 S1                                ; Set minimum Z

; Endstops
M574 X2 Y2 Z2 S1                          ; Set active high endstops

; Z-Probe
M558 P2 H5 F120 T6000                     ; Set Z probe type to modulated and the dive height + speeds
G31 P500 X-25 Y-13 Z2.5                   ; Set Z probe trigger value, offset and trigger height
M557 R140 S20                             ; Define mesh grid

; Heaters
M301 H0 S1.00 P10 I0.1 D200 T0.4 W180 B30 ; Use PID on bed heater (may require further tuning)
M305 P0 R4700 T100000 B4148 C2.117e-7	  ; Set thermistor + ADC parameters for bed heater
M143 H0 S120                              ; Set temperature limit for heater 0 to 120C
M305 P1 R4700 T100000 B4148 C2.117e-7	  ; Set thermistor + ADC parameters for heater 1
M143 H1 S285                              ; Set temperature limit for heater 1 to 285C
M305 P2 T100000 B4725 C7.060000e-8 R4700  ; Set thermistor + ADC parameters for heater 2
M143 H2 S285                              ; Set temperature limit for heater 2 to 285C

; Fans
M106 P0 S1 I0 F500 H-1                    ; Set fan 0 value, PWM signal inversion and frequency. Thermostatic control is turned off
M106 P1 S1 I0 F500 H-1                    ; Set fan 1 value, PWM signal inversion and frequency. Thermostatic control is turned off
M106 P2 S1 I0 F500 H-1                    ; Set fan 2 value, PWM signal inversion and frequency. Thermostatic control is turned off

; Tools
M563 P0 D0 H1                             ; Define tool 0
G10 P0 X0 Y0 Z0                           ; Set tool 0 axis offsets
G10 P0 R0 S0                              ; Set initial tool 0 active and standby temperatures to 0C
M563 P1 D1 H2                             ; Define tool 1
G10 P1 X0 Y9 Z0                           ; Set tool 1 axis offsets
G10 P1 R0 S0                              ; Set initial tool 1 active and standby temperatures to 0C

; Automatic saving after power loss is not enabled

; Custom settings are not configured

; Miscellaneous
M501                                      ; Load saved parameters from non-volatile memory