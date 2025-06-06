---
title: Modular Square Root
time: 2025-04-13 20:23:32
categories: [CTF, Cryptohack]
tags: [cryptography, legendres-symbol]
---

I will skip explanation of what is square root of a number modulo `P`. This article is more about finding the square root.

## Quadratic Nature

For all the numbers between `1` an `P-1`, only few numbers will possibly have a square root. Numbers with a square root
is called `Quadratic Residue` and numbers without a square root is `Quadratic Non-Residue`.
Identifying this nature will help in quickly finding the which number can have a square root.

## Implementation

### Simple approach

Simplest way to calculate the square root of a number `a` modulo `P` is to iterate over all the numbers between
`2` to `P - 1` and see which one is the square root of `a`.

Here is the python code to do that:

```python

def find_sqrt(val, M):
  roots = [ i for i in range(M) if pow(i, 2, M) == x]
  return roots

p=29
ints=[14,6,11]

for x in ints:
  val = find_sqrt(x, p)
  if len(val) > 0:
    print(f'x is quadratic residue with {x}^2 = {val}')
  else:
    print(f'x is quadratic non-residue')


```

### Legendres Symbol

When `P` becomes large, running the for loop takes a lot of time.
Using [Legendres Symbol](https://en.wikipedia.org/wiki/Legendre_symbol), it is easier to identify if the given number
is Quadratic Residual or not. In other words, if it has square root or not.

Summary of its definition is:

```
(a/P) = a^((P-1)/2) mod P
if (a/P) = 1, a is quadratic residue and a != 0 mod P
if (a/P) = -1, a is quadratic non-residue
if (a/P) = 0, a = 0 mod P
Hence, by finding the value of `a^((P-1)/2)` we can determine if `a` is Quadratic residue of not
```

```python

def is_quad_residue(a, P):
  a_by_P =  Pow(a, (P - 1)//2, P)

  if a_by_P == 1:
    return True

  if a_by_P == -1:
    return False

```

### Simplified Square Root

It is not always easy to find the modular square root of a number, but by putting a restriction on `P`.
it can be easier to find the square root.

If `P % 4 = 3` or `P = 4*i + 3, for all i as positive integers` then square root of all the Quadratic resuduals can be calculated using the formula

> ±a^((P + 1)/4)

```python

def sqrt_mod_P(a, P):
  """
  If n is in the form 4*i + 3 with i >= 1 (OR P % 4 = 3)
  And, If Square root of n exists, then it must be
          ±n^((P + 1)/4)
  """
  if P % 4 == 3:
    n1 = pow(a, (P+1)//4, p)
    n2 = pow(-1 * a, (P+1)//4, P)
    return (n1, n2)

```

### Tonelli Shank Algorithm

Previous algorithm was applicable only for half of the `P`. To solve the same problem for all the `P > 2`,
[Tonelli–Shanks algorithm](https://en.wikipedia.org/wiki/Tonelli%E2%80%93Shanks_algorithm) can be used.

It's implementation is directly copied from [RosettaCode - Tonelli Shank's Algorithm](https://rosettacode.org/wiki/Tonelli-Shanks_algorithm#Python)
I will add explanation, after going thorugh its details.

```python

def legendre(a, P):
    return pow(a, (P - 1) // 2, P)


def tonelli(n, P):
    assert legendre(n, P) == 1, "not a square (mod P)"
    q = P - 1
    s = 0
    while q % 2 == 0:
        q //= 2
        s += 1
    if s == 1:
        return pow(n, (P + 1) // 4, P)
    for z in range(2, P):
        if P - 1 == legendre(z, P):
            break
    c = pow(z, q, P)
    r = pow(n, (q + 1) // 2, P)
    t = pow(n, q, P)
    m = s
    t2 = 0
    while (t - 1) % P != 0:
        t2 = (t * t) % P
        for i in range(1, m):
            if (t2 - 1) % P == 0:
                break
            t2 = (t2 * t2) % P
        b = pow(c, 1 << (m - i - 1), P)
        r = (r * b) % P
        c = (b * b) % P
        t = (t * c) % P
        m = i
    return r
```


### Example

```python

# P = <large prime number> with property 'P % 4 = 3'
# ints = <list of randome integers>

for x in ints:
  val = is_quad_residue(x, P)
  if val == 1:
    print(f'x is quadratic resiue with {x}^2 = {sqrt_mod_P(x, P)}')
  else:
    print(f'x is quadratic non-resiue')


```
