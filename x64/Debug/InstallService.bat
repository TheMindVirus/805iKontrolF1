sc create 805iKontrolF1 binPath="%cd%/805iKontrolF1.exe service" start=auto
sc description 805iKontrolF1 "Controls the LEDs on the In-Win 805 Infinity Chassis with a Traktor Kontrol F1 MIDI Controller"
sc start 805iKontrolF1
pause