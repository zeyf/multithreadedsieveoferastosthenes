#include <iostream>
#include <vector>
#include <thread>
#include <cmath>
#include <chrono>

using namespace std;

#define NUMTHREADS 8

// This function is for the threads to run in parallel to mark the multiples' block ranges linearly by the prime step
void markComposite(vector<bool> &primes, int low, int high, int prime) {

    // Mark as composite!
    while (low <= high) {
        primes[low] = false;
        low += prime;
    };

};

int main () {

    // Get the start time
    auto started = std::chrono::high_resolution_clock::now();

    // Initialize the limit and the sqrt upper bound, as well as the prime vector
    int limit = 100000000, upperBound = sqrt(limit);

    // Note that all even numbers are marked as true.
    // For these indices, the true / false status is sentinel and has no meaning aside from 2.
    vector<bool> primes(limit+1, true);
    primes[0] = primes[1] = false;

    // Iterate over all odd numbers as 2 is already accounted for as a prime implicitly due to initialization of prime vector.
    for (int x = 3; x <= upperBound; x += 2) {

        // If the current index (representing a number in the range) is not false, we are at a prime.
        if (primes[x]) {

            // Where x is the current prime candidate, establish 8 blocks / ranges from [x * 2, limit]
            // To do this, we must establish how many multiples of x we want per block / range.
            // Since the greater the prime number, the greater the speed of which we iterate through a range linearly with that prime.
            // We must then factor this in along with the number of threads in order to accurately account for the multiples count of the prime per block.
            int approxMultiplesBlockSize = (limit - x) / (x * NUMTHREADS) + 1;

            // There will be 8 threads in every case for each number.
            vector<thread> threads;

            int currentMultiple = 1;

            // This will create the threads based on the range blocks of approxMultiplesBlockSize multiples per block.
            // To not mark the prime itself, we have a slight adjustment on the first multiple to start at the second. Otherwise, we go stepwise by multiples block size.
            // Bound the last one to a maximum of the limit if the last multiple before the next block sequence range starts after it.
            while (x * currentMultiple <= limit) {
                int blockStart = currentMultiple == 1 ? x * (currentMultiple + 1) : x * currentMultiple,
                    blockEnd = min(x * (currentMultiple + approxMultiplesBlockSize - 1), limit);

                thread t(markComposite, ref(primes), blockStart, blockEnd, x);
                threads.push_back(move(t));

                currentMultiple = currentMultiple + approxMultiplesBlockSize;
            };

            // Join all of the threads! We want complete all processing of the multiples block ranges for the current prime before we move on.
            for (thread &t: threads)
                t.join();

        };
    };

    // Initialize variables to count the number and sums of all primes in the range [1, 10^8], as well as the vector and indexes.
    long long primeSum = 0, primeCount = 0;
    vector<int> last10Primes(10, -1);
    int primeIndex = 0;
    
    // Iterate over all possible primes...
    for (int x = 2; x <= limit; x += x == 2 ? 1 : 2) {
        if (primes[x]) {
            // Add in the prime to the sum and count it, as well as add the prime to the last 10 list.
            primeSum += x;
            ++primeCount;

            // Cycle on to next spot for a following prime.
            last10Primes[primeIndex] = x;
            primeIndex = (primeIndex + 1) % 10;
        };
    };

    // Sort the last 10 Primes list. This is to be done just to have.
    sort(last10Primes.begin(), last10Primes.end());

    // Get the done time
    auto done = std::chrono::high_resolution_clock::now();

    // Calculate the execution time
    auto timeToExecute = std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count();

    // Print out the count and the sum.
    cout << timeToExecute << "ms " << primeCount << " " << primeSum << "\n";
    // Print out the list of the last 10 primes.
    for (int p: last10Primes) {
        if (p != -1)
            cout << p << " ";
    };

    cout << "\n";

    return 0;
};