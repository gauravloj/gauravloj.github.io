---
title: Crypto Commands
time: 2025-01-13 13:04:09
categories: [Cyber Security, CTFs]
tags: [cryptography, rsa, aes, sha, md5, hashes]
---

Useful commands to handle different cryptographic scenarios

## Symmetric encryption

### GPG

```sh
# encrypt a file using GnuPG (GPG):
# here CIPHER is the name of the encryption algorithm. You can check supported ciphers using the command gpg --version.
# The encrypted file will be saved as message.txt.gpg.
gpg --symmetric --cipher-algo CIPHER message.txt


# create an ASCII armoured output, which can be opened in any text editor,
# add the option --armor
gpg --armor --symmetric --cipher-algo CIPHER message.txt

# decrypt:
gpg --output original_message.txt --decrypt message.gpg

```

### OpenSSL

```sh
# encrypt a file using OpenSSL:
openssl aes-256-cbc -e -in message.txt -out encrypted_message

#  decrypt the encrypted file
openssl aes-256-cbc -d -in encrypted_message -out original_message.txt

# To make the encryption more secure and resilient against brute-force attacks,
# we can add -pbkdf2 to use the Password-Based Key Derivation Function 2 (PBKDF2);
# moreover, we can specify the number of iterations on the password to derive the
# encryption key using -iter NUMBER. To iterate 10,000 times, the previous command would become:
openssl aes-256-cbc -pbkdf2 -iter 10000 -e -in message.txt -out encrypted_message

# Decryption for file encrypted using above command
openssl aes-256-cbc -pbkdf2 -iter 10000 -d -in encrypted_message -out original_message.txt

```

## Asymmetric encryption

```sh
# use genrsa to generate an RSA private key.
# 2048: key size of 2048 bits.
openssl genrsa -out private-key.pem 2048

# -pubout: get the public.
# -in: set the private key as input
openssl rsa -in private-key.pem -pubout -out public-key.pem

# see real RSA variables.
openssl rsa -in private-key.pem -text -noout:

# Encryot message using a public key
openssl pkeyutl -encrypt -in plaintext.txt -out ciphertext -inkey public-key.pem -pubin

# Decrypt ciphertext using private key
openssl pkeyutl -decrypt -in ciphertext -inkey private-key.pem -out decrypted.txt

# View Public key details
ssh-keygen -lf /path/to/key.pub

```

## Key Exchange

```sh
# Specify dhparam to indicate Diffie-Hellman key exchange parameters
# along with the specified size in bits, such as 2048 or 4096.
openssl dhparam -out dhparams.pem 2048

# view the prime number P and the generator G using the command
openssl dhparam -in dhparams.pem -text -noout

```

## Hashing

```sh

# Generate HMAC hash
openssl sha256 -hex -mac HMAC -macopt key:<key here> file.txt

```

## Certificates

```sh
# Generate a certificate signing request
# req -new - create a new certificate signing request
# -nodes - save private key without a passphrase
# -newkey - generate a new private key
# rsa:4096 - generate an RSA key of size 4096 bits
# -keyout - specify where to save the key
# -out - save the certificate signing request
openssl req -new -nodes -newkey rsa:4096 -keyout key.pem -out cert.csr


# generate a self-signed certificate.
# The -x509 indicates that we want to generate a self-signed certificate instead
# of a certificate request. The -sha256 specifies the use of the SHA-256 digest.
# It will be valid for one year as we added -days 365.
openssl req -x509 -newkey -nodes rsa:4096 -keyout key.pem -out cert.pem -sha256 -days 365

# view  certificate:
openssl x509 -in cert.pem -text
```

## Signing Emails

```sh
# Generate public and private keys
gpg --gen-key

# Encrypt the message
gpg --encrypt --sign --armor -r strategos@tryhackme.thm message.txt

```
