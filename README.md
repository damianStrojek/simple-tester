# simpleTester 3.0 Release

Simple tester inspired by the source code of @Lemm Tester. Basic version has a `system("clear")` command implemented because I prefer to work on Linux. If you want it to work properly on Windows change it to `system("cls")`.

## Compile GNU/Linux

```bash
g++ SimpleTester.cpp -o simpleTester
```

## Improvements

- The questions are randomly selected from the database - if you answered a given question correctly, it goes to the end of the queue and should not appear soon, but if you answered the question incorrectly, it will appear again soon.
- The answers are also randomly selected, so you always have to read them and enter a different answer.
- The answer is in the format `a`, `ab`, `bc`, `dec`, etc. Before submitting it is sorted and all white spaces are deleted so you can type it in every combination you would like and it should work.

## Database
In order for it to work you need to provide a `.txt` database file. If you are worried about state of your database you can always use the `checkQuestions(questions)` function.

Template of a question looks like this:

```c#
<question>
<number of answers>
<answer nr n1>
<answer nr n2>
<answer nr n3>
<answer nr n4>
<proper answer>
<ENTER>
```

For example:

```
W przypadku klas w Javie (1 poprawna):
4
Nie istnieje możliwość implementowania wielu interfejsów
Nie istnieje możliwość dziedziczenia po wielu klasach bazowych
Wszystkie metody klasy bazowej muszą być nadpisane w klasie potomnej
Żadne z powyższych
b

W przypadku kolekcji TreeSet (1 poprawna):
4
Wyszukiwanie elementów cechuje się złożonością liniową O(n)
Wyszukiwanie elementów cechuje się złożonością logarytmiczną O(log n)
Wyszukiwanie elementów odbywa się w czasie stałym O(1)
Dane mają postać par (klucz -> wartość)
b

```