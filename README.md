# Ewolucja 3D

## Opis projektu
Szkielet aplikacji składa się z części wizualno-fizycznej wykonanej w silniku Godot, wyświetlającej kulkę odbijającą się od podłoża oraz z części związanej ze sztuczną inteligencją wykorzystującą bibliotekę libtorxh.

## Zależności
Do budowania projektu wykorzystywane jest narzędzie scons.

Aplikacja wykorzystuje silnik Godot w wersji 3.5. Można go pobrać poa adresem:
https://github.com/godotengine/godot/releases/tag/3.5.3-stable

Wykorzytywane są także biblioteki Catch2 oraz libtorch, które są instalowane automatycznie.

## Sposób urchomienia
### Budowanie - Linux
W głównym katalogu projektu wykonaj:  
`scons platform=linux`

### Uruchomienie projektu Godot
Uruchom silnik Godot. W widoku listy projektów, wybierz opcję "Zaimportuj" i wskaż plik project.godot z katalogu project. Uruchom projekt klikając F5.

### Uruchamianie sieci neuronowej
Przejdź do katalogu `ArtifficalInteligenceModels/build`.  
Wykonaj:  
`./ai`

Rozpocznie to uczenie prostej sieci neuronowej.

### Uruchomiene testów jednostkowych
Przejdź do katalogu `tests/build`.  
Wykonaj:  
`./tests`
