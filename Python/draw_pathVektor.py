import matplotlib.pyplot as plt # type: ignore

# Definition der Schritte pro cm und der initialen Variablen
stepsPerCm = 1  # Anzahl der Schritte für 1 cm
step1Cm = stepsPerCm  # Schritte für 1 cm
count = 0

# Startpunkt bei (-20, -20) cm
current_x = 20
current_y = -20

# Listen für die Punkte (x und y) der Vektoren
x_points = [current_x]
y_points = [current_y]

def moveX(steps, direction, speed):
    global current_x, current_y
    if direction == "LOW":  # Nach links
        current_x -= steps / stepsPerCm
    else:  # Nach rechts
        current_x += steps / stepsPerCm
    x_points.append(current_x)
    y_points.append(current_y)

def moveY(steps, direction, speed):
    global current_x, current_y
    if direction == "LOW":  # Nach oben
        current_y += steps / stepsPerCm
    else:  # Nach unten
        current_y -= steps / stepsPerCm
    x_points.append(current_x)
    y_points.append(current_y)

def moveDiagonal(steps, xDirection, yDirection, speed):
    global current_x, current_y
    if xDirection == "HIGH":  # X nach rechts
        current_x += steps / stepsPerCm
    else:  # X nach links
        current_x -= steps / stepsPerCm
    
    if yDirection == "LOW":  # Y nach oben
        current_y += steps / stepsPerCm
    else:  # Y nach unten
        current_y -= steps / stepsPerCm
    
    x_points.append(current_x)
    y_points.append(current_y)

def moveLinear(vektoren, motorDelayFast):
    # Schritte pro Zentimeter (421 Schritte pro cm)
    stepsPerCm = 1
    
    # Startpunkt bei (-20, -20) cm
    current_x = -20
    current_y = -20
    
    # Listen für die Punkte (x und y) der Vektoren
    x_points = [current_x]
    y_points = [current_y]
    
    for (xRichtung, yRichtung) in vektoren:
        # Konvertiere die Schritte in Zentimeter
        xCm = xRichtung / stepsPerCm
        yCm = yRichtung / stepsPerCm
        
        # Berechne den neuen Endpunkt basierend auf dem vorherigen Punkt
        current_x += xCm
        current_y += yCm
        
        # Füge den neuen Punkt der Liste hinzu
        x_points.append(current_x)
        y_points.append(current_y)
    
    # Zeichne den Pfad
    draw_path(x_points, y_points)

# Erster Schleifenabschnitt (20 Wiederholungen)
for i in range(20):
    print(f"1 cm nach links Nr. {i + 1}")
    moveX(step1Cm, "LOW", 800)  # Richtung nach links
    count += 1

    print(f"Bewege diagonal nach rechts oben")
    moveDiagonal(step1Cm * count, "HIGH", "LOW", 800)  # X nach rechts, Y nach oben

    print(f"1 cm nach oben Nr. {i + 1}")
    moveY(step1Cm, "LOW", 800)  # Richtung nach oben
    count += 1

    print(f"Bewege diagonal nach links unten")
    moveDiagonal(step1Cm * count, "LOW", "HIGH", 800)  # X nach links, Y nach unten

# Zweiter Schleifenabschnitt (20 Wiederholungen)
for i in range(20):
    print(f"1 cm nach oben Nr. {i + 1}")
    moveY(step1Cm, "LOW", 800)  # Richtung nach oben
    count -= 1

    print(f"Bewege diagonal nach rechts oben")
    moveDiagonal(step1Cm * count, "HIGH", "LOW", 800)  # X nach rechts, Y nach oben

    print(f"1 cm nach links Nr. {i + 1}")
    moveX(step1Cm, "LOW", 800)  # Richtung nach links
    count -= 1

    print(f"Bewege diagonal nach links unten")
    moveDiagonal(step1Cm * count, "LOW", "HIGH", 800)  # X nach links, Y nach unten

# Zeichne den Pfad
def draw_path(x_points, y_points):
    # Setze die Größe des Fensters auf 40 x 40 cm (entspricht ~15.75 x 15.75 Zoll)
    plt.figure(figsize=(16.14, 16.14))  # 40 cm in Zoll (40 / 2.54 = 15.75)
    
    # Zeichne den Pfad der Vektoren
    plt.plot(x_points, y_points, marker='o', color='r')
    
    # Setze die Achsenlimits auf -20 bis +20 cm, um den 40 x 40 cm Bereich zu visualisieren
    plt.xlim(-21, 21)
    plt.ylim(-21, 21)
    
    # Achsenbeschriftung und Raster
    plt.grid(True)
    plt.axhline(0, color='black', linewidth=0.5)
    plt.axvline(0, color='black', linewidth=0.5)
    plt.xlabel('X-Achse (cm)')
    plt.ylabel('Y-Achse (cm)')
    plt.title("Bewegungen aus dem Arduino Code in einem 40x40 cm Fenster")
    
    # Pfad anzeigen
    plt.show()

# Pfad zeichnen
draw_path(x_points, y_points)

# Beispiel: Liste von Vektoren (Schritte in X und Y)
vektoren = [
    (10 , 0 ),  # 10 cm auf X, 20 cm auf Y
    (0 , 10 ), # -5 cm auf X, -10 cm auf Y
    (-10 , 0 ),    # 8 cm auf X, 15 cm auf Y
    (10 , 0 ),  # 10 cm auf X, 20 cm auf Y
    (0 , 10 ), # -5 cm auf X, -10 cm auf Y
    (-10 , 0 ),

]

# Funktion aufrufen
moveLinear(vektoren, 0)