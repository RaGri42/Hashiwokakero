    Einlesen einer Instanz aus den Formaten .plain und .xy sowie das 
    Einlesen von Lösungen im Format .xy.solution. (Instanzen)
    
    Bedienung über die Maus.

    Wenn ein Lösungsfile (.xy.solution) angegeben wird, ist dieses als Klickfolge auf Inseln zu verstehen, 
    das Spiel sollte entsprechende Brücken bauen oder über ungültige Informieren.
    Konsolengrafik: Um den Klickbereich zu vergrößern soll jede Insel aus mindestens 3x3 Feldern bestehen.
    Farbliche Markierung wenn eine Insel die korrekte Anzahl Brücken besitzt und falls zu viele Brücken vorhanden sind.
    Undo Funktion, wobei die Anzahl Züge die man höchstens hintereinander rückgängig machen 
    kann ein Argument in der Kommandozeile sein sollte (Default: 5).
    Alle Funktionalität, die mit Ein- bzw. Ausgabe zu tun hat, sollte sauber getrennt von den anderen 
    Teilen in eigenen Methoden stehen und diese Methoden brauchen nicht getestet zu werden. 
    Für alle anderen Methoden soll es selbstverständlich wie gehabt einen Unit Test geben.

    Es soll selbstverständlich auf alles geachtet werden, was Sie in der Vorlesung gelernt haben: gutes 
    Klassendesign, Trennung in .h und .cpp Dateien, const correctness, 
    valgrind soll ohne Fehler durchlaufen, allgemein die 10 Gebote des Ü1. 

Anforderungen (optional): Hier ein paar Ideen für optionale Erweiterungen:

    Generieren von neuen Instanzen (wenn diese konstruktiv erzeugt werden ist ein Solver nicht nötig)

    Zur Laufzeit umschaltbarer Markierungsmodus der alle möglichen Nachbarninsel hervorhebt. 
    Im Idealfall ohne das die Insel angeklickt wurde sondern nur durch drüberfahren und h drücken.
    
    Einlesen mehrerer Instanzen + ein Menu im Programm, in dem man die Instanz auswählen kann + Möglichkeiten, die Instanzen der Reihe nach zu lösen.
    
    Prüfen ob die Klickfolge minimal war.
    Farben benutzerdefiniert machen.

    Ausgeben der Spielerzugfolge als Lösung im Format .xy.solution.
    Animiertes darstellen einer Lösung.
    Was immer Ihnen noch (Sinnvolles und Sinnloses) einfällt. 
