# Multithreaded Sieve of Erastosthenes

by Zain Emmanuel Yousaf Fuentes

## File name and how to run it

The file name is 'main.cpp'. In order to run it and ensure correct compiler method relative to what I was using, please execute the following command:

```
g++ main.cpp -std=c++17
```

Then, run the executable generated 'a.out' via the following command:

```
./a.out
```

## The Problem

Your non-technical manager assigns you the task to find all primes between 1 and 
10^8.  The assumption is that your company is going to use a parallel machine that 
supports eight concurrent threads. Thus, in your design you should plan to spawn 8 
threads that will perform the necessary computation. Your boss does not have a strong 
technical background but she is a reasonable person. Therefore, she expects to see that 
the work is distributed such that the computational execution time is approximately 
equivalent among the threads. Finally, you need to provide a brief summary of your 
approach and an informal statement reasoning about the correctness and efficiency of 
your design. Provide a summary of the experimental evaluation of your approach. 
Remember, that your company cannot afford a supercomputer and rents a machine by 
the minute, so the longer your program takes, the more it costs. Feel free to use any 
programming language of your choice that supports multi-threading as long as you 
provide a ReadMe file with instructions for your manager explaining how to compile and 
run your program from the command prompt.

## Program Design and Correctness

At any point throughout the program, an average and maximum of 8 threads are running concurrently in order to find the primes in the range [1, 10^8].

The primary intuition used was the following formula I derived:

$$approximateMultiplesPerBlock = \lfloor \frac{LIMIT - PRIME}{PRIME * NUMTHREADS} \rfloor + 1$$

This allows for the segmentation of a prime number's multiples from $$[PRIME, LIMIT]$$ into 8 blocks, and how many multiples of a prime that block should contain on average.

For example, if we are at the prime number 5 and our limit is 111:

$$approximateMultiplesPerBlock = \lfloor \frac{100 - 5}{5 * 8} \rfloor + 1$$
Next step:
$$approximateMultiplesPerBlock = \lfloor \frac{95}{40} \rfloor + 1$$
Next step:
$$approximateMultiplesPerBlock = (2) + 1$$
Next step:
$$approximateMultiplesPerBlock = 3$$

So, now we are looking to have 3 multiples of the prime number (5) per bucket.

Therefore, we will have the following buckets:

$$[10,15][20,30][35,45][50,60][65,75][80,90][95,100],[105,111]$$

Note that we have not started at the prime number itself only in the first example (5) so we do not mark this value as composite.
Also note that the last value, if we would have included 3 multiples of 5 from [105, ...] we would actually go over the limit, say with a range from [105, 115], hence we control the upper bound like this. Either way, the limit will be hit if it is a multiple of the initial prime number.

The idea now is to use multithreading on all of these ranges across 8 threads.

We know that all the values from any given block range $$[start, end]$$ are composite as long as we go through values from $$ start <= end $$, stepping by prime (aka 5) each time.

Example:

Thread 1: $$[10,15] -> 10, 15$$ (again, bumped by one to exclude prime itself)
--
Thread 2: $$[20,30] -> 20, 25, 30$$
--
Thread 3: $$[35, 45] -> 35, 40, 45$$
--
... and so on.

## Efficiency

The algorithm is extremely efficient, running on average in less than 1 second using on average and at most 8 threads at any point throughout the prime marking step as we end up using multiple optimizations on top of this.

It is well known that the Sieve of Erasthosthenes is a relatively efficient and surprisingly simple solution to something interesting and obscure like prime numbers, however there some simple mathematical observations that can be made and used in order to ensure a better overall run time and / or time complexity, along with decreasing the "ignored" yet still present high constant that comes up in the classical Sieve of Erasthosthenes.

The mathematical optimizations:
1. Iterating through a possible primes up to $$[2, \sqrt{LIMIT}]$$ and marking all of their multiples up to $$ <= LIMIT$$
2. Decreasing the constant by half by excluding any processing of evens (2 is the only prime number and this case was handled implicitly in terms of marking in the algorithm)
3. Also, iterating through only odd multiples of prime numbers decreased this constant down by half as well as we did not have to consider any of the even multiples as we consider all evens except for 2 as sentinel.
