# Question

Write a multi-threaded C program that gives readers priority over writers concerning a shared (global) variable.

Essentially, if any readers are waiting, then they have priority over
writer threads <br/>
1. Writers can only write when there are no readers.


This program should adhere to the following constraints:</br>
1. Multiple readers/writers must be supported (5 of each is fine)</br>
2. Readers must read the shared variable X number of times</br>
3. Writers must write the shared variable X number of times</br>
4. Readers must print:</br>
i. The value read</br>
ii. The number of readers present when value is read</br>
5. Writers must print:</br>
i.The written value</br>
ii. The number of readers present were when value is written (should be 0)</br>
6. Before a reader/writer attempts to access the shared variable it should wait some random amount of time <br/>


Note: This will help ensure that reads and writes do not occur all at once


Use pthreads, mutexes, and condition variables to synchronize access to the shared variable.&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
