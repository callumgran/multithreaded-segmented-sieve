# Multithreaded Segmented Sieve of Eratosthenes
This is a multithreaded implementation of the segmented sieve of Eratosthenes algorithm. It is based on the segmented sieve algorithm described in the paper [An Introduction To Prime Number Sieves](https://research.cs.wisc.edu/techreports/1990/TR909.pdf) by Jonathan Sorenson.

## Building
This project has a small testfile that can be compile by running `make segmented_sieve`, resulting in an executable called `segmented_sieve`. This executable can be run with the following command:
```
./segmented_sieve <start> <end> <threads>
```
Where `<start>` is the starting number of the range, `<end>` is the ending number of the range and `<threads>` is the number of threads to use. The program will then print all prime numbers in the range `[<start>, <end>]` to `stdout`.

## Performance
For the performance tests, the program was run on a machine with an AMD Ryzen 7 4800u processor with 8 cores and 16 threads. The program was run with the following commands with the following results:
```
./segmented_sieve 1 1000000000 1
```
Time used: 13618.4 ms
```
./segmented_sieve 1 1000000000 2
```
Time used: 12420.9 ms
```
./segmented_sieve 1 1000000000 4
```
Time used: 10868.9 ms
```
./segmented_sieve 1 1000000000 8
```
Time used: 9370.3 ms
```
./segmented_sieve 1 1000000000 16
```
Time used: 8527.4 ms

### Regular sieve of Eratosthenes

By using the `make segmented_sieve_eratosthenes` command, a version of the program using the segmented and the regular sieve of Eratosthenes algorithm is compiled. To compare, the regular sieve of Eratosthenes used the following times:
```
./segmented_sieve 1 1000000000 1
```
Time used: 11545.8 ms