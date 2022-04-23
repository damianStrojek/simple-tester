# simpleTester
Simple tester made from the source code of @Lemm. 

# Database
In order for it to work you need to provide a "baza.txt" ANSI encoded file. 
The questions look like this:
```c#
<number of question>.<question>
<number of answers>
<answer nr 1>
<answer nr 2>
<answer nr 3>
<answer nr 4>
<proper answer>
<enter>
```
For example:
```
1. W przypadku klas w Javie (1 poprawna):
4
Nie istnieje możliwość implementowania wielu interfejsów
Nie istnieje możliwość dziedziczenia po wielu klasach bazowych
Wszystkie metody klasy bazowej muszą być nadpisane w klasie potomnej
Żadne z powyższych
b

2. W przypadku kolekcji TreeSet (1 poprawna):
4
Wyszukiwanie elementów cechuje się złożonością liniową O(n)
Wyszukiwanie elementów cechuje się złożonością logarytmiczną O(log n)
Wyszukiwanie elementów odbywa się w czasie stałym O(1)
Dane mają postać par (klucz -> wartość)
b

```
