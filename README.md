# Simple unix local socket perf test

```bash
$ python3 server.py &
$ g++ main.cpp && ./a.out
```

# My results

## Server side (Throughput)

Each image is 256x256x4x4 bytes

```
4025 images in 10 sec
```

## Client side (Round trip delay)

```
Min Delay : 906500ns
Max Delay : 2982200ns
Avg Delay : 2.36723e+06ns
```
