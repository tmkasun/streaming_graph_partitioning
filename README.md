# streaming_graph_partitioning
Streaming Graph partitioning

![CodeCogsEqn](https://user-images.githubusercontent.com/3313885/58380863-5eb18f80-7fd4-11e9-9214-3136c6275559.png)

\left |N(v)\cap {S_i}  \right | - {\partial c}(\left | {S_i}  \right |) 
https://www.codecogs.com/latex/eqneditor.php

{\partial c}(\left | {S_i}  \right |) = {\alpha}rx^{r-1}

![CodeCogsEqn (1)](https://user-images.githubusercontent.com/3313885/58380913-3b3b1480-7fd5-11e9-9b50-5eea3d9e78d3.png)


![fennel](https://user-images.githubusercontent.com/3313885/59002385-d8951480-882f-11e9-89f7-c9d006a97d77.gif)


```
 ***Received the end of stream
0 => Vertext count = 5
0 => Edges count = 4
0 => Edge cuts count = 4
0 => Cut ratio = 0.5
Printing edge cuts of 0 partition
8 ____
        | ---> 9


39 ____
        | ---> 36


14 ____
        | ---> 17


35 ____
        | ---> 34


Printing edge list of 0 partition
6 ____
        | ===> 8


7 ____
        | ===> 8




14 ____
        | ===> 16
        | ===> 29






1 => Vertext count = 5
1 => Edges count = 3
1 => Edge cuts count = 6
1 => Cut ratio = 0.666667
Printing edge cuts of 1 partition
9 ____
        | ---> 8


36 ____
        | ---> 39


19 ____
        | ---> 20


36 ____
        | ---> 37


14 ____
        | ---> 13


36 ____
        | ---> 38


Printing edge list of 1 partition
18 ____
        | ===> 19




35 ____
        | ===> 36




41 ____
        | ===> 42




2 => Vertext count = 4
2 => Edges count = 1
2 => Edge cuts count = 3
2 => Cut ratio = 0.75
Printing edge cuts of 2 partition
20 ____
        | ---> 19


37 ____
        | ---> 36


15 ____
        | ---> 13


Printing edge list of 2 partition
9 ____
        | ===> 10




3 => Vertext count = 4.5
3 => Edges count = 3
3 => Edge cuts count = 5
3 => Cut ratio = 0.625
Printing edge cuts of 3 partition
17 ____
        | ---> 14


34 ____
        | ---> 35


13 ____
        | ---> 14


38 ____
        | ---> 36


13 ____
        | ---> 15


Printing edge list of 3 partition
5 ____
        | ===> 13


12 ____
        | ===> 13




33 ____
        | ===> 34




Time taken = 28433 micro seconds
```
