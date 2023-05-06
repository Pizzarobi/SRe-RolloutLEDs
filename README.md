# SRe Unterboden LEDs
Programm zur Ansteuerung von zwei LED Strips mit jeweils 42 RGB "Pixels". Diese sollen den Unterboden beleuchten und nice aussehen.

## Hardware
- ESP8266
- 84 RGB LEDs
- Voltage Converter 24v - 5v

# Pattern Ideen

## 1. Laufband (Farbig)
Ein Farbiger Pixel soll sich ums Auto bewegen. Der hellste ist auf currPos. currPos wird bei jedem Stepp um eins verschoben. Alle n Pixel hinter currPos werden immer dunkler. Je nach Strip länge und Geschmack variierbar. <br>
Zum Beispiel: n = Hellster Pixel. n-1 ist immer noch sichtbar hell aber viel Dunkler. => "Falloff" der helligkeit je nach Gehschmack

## 2. Rainbow Wave
Regenbogen der sich immer um n+1 bewegt.

## 3. Sparkle (Schanzerfarbig)
Random Stern sparkle mit random helligkeiten.

## 4. Laola Welle
Falls die Strips z.B. Seitlich sind gleichzeitige Waves/Laufband. Beide Seiten Synchron.

## Sonstiges.
2 Band Modi. gespiegelt oder nicht. => Beide Strips gleich oder nicht


<br><br><br>
### Made by
Robert Kalmar
