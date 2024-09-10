import matplotlib.pyplot as plt

def moveLinear(vektoren, motorDelayFast):
    # Schritte pro Zentimeter (421 Schritte pro cm)
    stepsPerCm = 1
    
    # Startpunkt (Ursprung)
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

def draw_path(x_points, y_points):
    # Setze die Größe des Fensters auf 40 x 40 cm (entspricht ~15.75 x 15.75 Zoll)
    plt.figure(figsize=(15.75, 15.75))  # 40 cm in Zoll (40 / 2.54 = 15.75)
    
    # Zeichne den Pfad der Vektoren
    plt.plot(x_points, y_points, marker='o', color='r')
    
    # Setze die Achsenlimits auf -20 bis +20 cm, um den 40 x 40 cm Bereich zu visualisieren
    plt.xlim(-20, 20)
    plt.ylim(-20, 20)
    
    # Achsenbeschriftung und Raster
    plt.grid(True)
    plt.axhline(0, color='black', linewidth=0.5)
    plt.axvline(0, color='black', linewidth=0.5)
    plt.xlabel('X-Achse (cm)')
    plt.ylabel('Y-Achse (cm)')
    plt.title("Mehrere Vektoren in einem 40x40 cm Fenster")
    
    # Pfad anzeigen
    plt.show()

# Beispiel: Liste von Vektoren (Schritte in X und Y)
vektoren = [
    (10, 0), 
    (0, 10), 
    (-10, 0),
    (10, 0),  
    (0, 10),
    (-10, 0)
]

# Funktion aufrufen
moveLinear(vektoren, 0)