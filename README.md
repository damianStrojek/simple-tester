# simpleTester 3.0 Release

"Simple Tester" inspired by the source code of @Lemm Tester. 

The program is made for students who want to learn abcd questions for the exam. The databases in the repository are prepared for one of the main Computer Science courses that have the abcd tests at the Gdańsk University of Technology.

Basic version has a `system("clear")` command implemented because I prefer to work on Linux. If you want it to work properly on Windows change it to `system("cls")`.

## Improved functions

- The questions are randomly selected from the database - if you answered a given question correctly, it goes to the end of the queue and should not appear soon, but if you answered the question incorrectly, it will appear again soon.
- Asnwers are also randomly selected, so you always have to read them and enter a different answer.
- One answer is in the format `a`, `ab`, `bc`, `dec`, etc. Before submitting, it is sorted and all white spaces are deleted so you can type it in every combination you would like and it should work.
- I've added a function that gives the information about amount of the correct answers. 
- There is a new type of questions - "open questions". Now you can learn how to answer open questions correctly. I didn't introduce any type of buffer for wrong questions - the answer has to be 100% perfect.

## Compile GNU/Linux

```bash
g++ simple-tester.cpp -o simple-tester
chmod +x simple-tester
```

## Database Source File

In order for it to work you need to provide a `.txt` database file. If you are worried about state of your database you can always use the `checkQuestions(questions);` function.

Template of a question looks like this:

```c++
// abcd questions
<question>
<number of answers>
<answer nr n1>
<answer nr n2>
<answer nr n3>
<answer nr n4>
<proper answer>
<ENTER>

// open questions
<question>
o
<answer>
<ENTER>
```

For example:

```
W przypadku klas w Javie:
4
Nie istnieje możliwość implementowania wielu interfejsów
Nie istnieje możliwość dziedziczenia po wielu klasach bazowych
Wszystkie metody klasy bazowej muszą być nadpisane w klasie potomnej
Żadne z powyższych
b

W przypadku kolekcji TreeSet:
4
Wyszukiwanie elementów cechuje się złożonością liniową O(n)
Wyszukiwanie elementów cechuje się złożonością logarytmiczną O(log n)
Wyszukiwanie elementów odbywa się w czasie stałym O(1)
Dane mają postać par (klucz -> wartość)
b


```

## Known Problems

Sometimes there is a problem with the source file. On Linux I've observed that the file has to be configured with **End of Line Sequence** that equals to `LF` (right bottom corner when editing file in Visual Studio Code).

## Multi-line Questions

If you have multi-line questions in your database file you need to somehow check if you are still reading question or an answer. I have deleted this function from the main release and pasted it here for the future.

```c++
void loadQuestionsAnswers(std::ifstream &databaseFile, std::vector<Question> &questions, 
							std::string _question, int &numberOfQuestions){
    [...]           
    // Check if the first symbol is a number
    // This while is to make sure that the "multiple line questions" will get processed too
    while (!((int)temp[0] > 48 && (int)temp[0] < 57)) { 
        if (!((int)temp[0] > 48 && (int)temp[0] < 57)) _question += '\n' + temp;
        std::getline(databaseFile, temp);
    }
}
```