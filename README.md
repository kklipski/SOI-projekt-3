# Ćwiczenie 3.

## Synchronizacja procesów z wykorzystaniem semaforów

### Cel ćwiczenia
Należy napisać program w języku C w środowisku systemu Linux realizujący rozwiązanie problemu zadanego przez prowadzącego ćwiczenie. Problem jest związany z organizacją *N* buforów komunikacyjnych, pomiędzy którymi są narzucone dodatkowe więzy.

### Bufor komunikacyjny
Bufor komunikacyjny jest strukturą danych mieszczącą maksymalnie *M* elementów jednakowego typu. Elementy z bufora mogą być wyjmowane w kolejności umieszczania (FIFO) lub odwrotnej (LIFO, stos).

### Zadanie do zrealizowania
Należy zrealizować typ "bufor komunikacyjny". W czasie implementacji należy zapewnić synchronizację:
- nie dopuścić do czytania z pustego bufora,
- nie dopuścić do zapisu do pełnego bufora,
- zadbać o "nieprzeszkadzanie sobie" procesów zapisujących do bufora i procesów czytających z bufora.
  
Dodatkowe więzy mogą dotyczyć liczby elementów aktualnie przechowywanych w buforach, kolejności zapisu do i odczytu z poszczególnych buforów, liczby elementów zapisywanych/odczytywanych jednocześnie, operacji na wielu buforach jednocześnie itd.

### Przygotowanie do zajęć
- proszę zapoznać się z definicją semafora oraz przykładem zastosowania w problemie producent-konsument ze slajdów z wykładu. W szczególności są Państwo zobligowani znać podaną na slajdach dokładną definicję semafora. Proszę nie pomylić definicji z realizacją - [wykład 4.](w04.pdf) (w szczególności slajdy: "Semafory: definicja" i "Semafory: producent-konsument"),
- http://pl.wikipedia.org/wiki/Semafor_(informatyka),
- [*$ man semop*, *$ man semget*, *$ man ipcs*, *$ man ipcrm*](https://www.mimuw.edu.pl/~mengel/jpp/sem/readme.txt),
- *$ man 2 fork*,
- https://www.gnu.org/software/libc/manual/html_node/Process-Creation-Example.html.

#

### Dodatkowe informacje
Powyższy tekst pochodzi z instrukcji do trzeciego ćwiczenia laboratoryjnego z przedmiotu Systemy operacyjne (I). Treść mojego zadania jest umieszczona [poniżej](https://github.com/kklipski/SOI-projekt-3#moje-zadanie).

### Moje zadanie
Jest danych 2 producentów, którzy produkują losowe liczby i umieszczają je w 9-cio elementowym buforze FIFO. Jest też danych 2 konsumentów, którzy konsumują produkty z bufora (usuwają je), przy czym konsument pierwszy konsumuje tylko liczby parzyste, a drugi tylko liczby nieparzyste. Ponadto konsumenci mogą konsumować tylko, jeśli w buforze znajdują się co najmniej 3 produkty. Zadanie należy wykonać korzystając z semaforów.

Koncepcja rozwiązania: [Koncepcja.pdf](Koncepcja.pdf).

*Migrated from Bitbucket.*
