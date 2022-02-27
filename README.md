# ECC Encryption / Decryption

In this section we have implemented the implementation of **elliptic-curve based public-key encryption / decryption** \(asymmetric encryption scheme based on ECC\). This is **non-trivial** and usually involves a design of hybrid encryption scheme, involving ECC cryptography, ECDH key exchange and symmetric encryption algorithm.

Assume we have a ECC **private-public key pair**. We want to encrypt and decrypt data using these keys. By definition, **asymmetric encryption** works as follows: if we **encrypt data by a private key**, we will be able to **decrypt** the ciphertext later by the corresponding **public key**


Assume we have a **cryptographic elliptic curve** over finite field, along with its generator point **G**. We can use the following two functions to calculate a **shared a secret key** for **encryption** and **decryption** \(derived from the ECDH scheme\):

* **calculateEncryptionKey**\(pubKey\) --&gt; \(sharedECCKey, ciphertextPubKey\)
  1. Generate **ciphertextPrivKey** = _new **random** private key_.
  2. Calculate **ciphertextPubKey** = ciphertextPrivKey \* G.
  3. Calculate the ECDH shared secret: **sharedECCKey** = pubKey \* ciphertextPrivKey.
  4. Return both the **sharedECCKey** + **ciphertextPubKey**. Use the **sharedECCKey** for symmetric encryption. Use the randomly generated **ciphertextPubKey** to calculate the decryption key later.
* **calculateDecryptionKey**\(privKey, ciphertextPubKey\) --&gt; sharedECCKey
  1. Calculate the the ECDH shared secret: **sharedECCKey** = ciphertextPubKey \* privKey.
  2. Return the **sharedECCKey** and use it for the decryption.

Let us recall that EC points have the following property:

* \(_**a**_ \* **G**\) \* _**b**_ = \(_**b**_ \* **G**\) \* _**a**_

Now, assume that _**a**_ = privKey, _**a**_ \* **G** = pubKey, _**b**_ = ciphertextPrivKey, _**b**_ \* **G** = ciphertextPubKey.

The above equation takes the following form:

* pubKey \* ciphertextPrivKey = ciphertextPubKey \* privKey = **sharedECCKey**


Let's implement a fully-functional **asymmetric ECC encryption and decryption** hybrid scheme. It will be based on the `brainpoolP256r1` curve and the **AES-256-GCM** authenticated symmetric cipher.

We shall use the `tinyec` and `pycryptodome` Python libraries respectively for ECC calculations and for the AES cipher:

```text
pip install tinyec
pip install pycryptodome
```
The given code starts from generating an ECC public + private **key pair** for the message recipient: `pubKey` + `privKey`, using the `tinyec` library. These keys will be used to **encrypt** the message `msg` through the hybrid encryption scheme \(asymmetric ECC + symmetric AES\) and to **decrypt** is later back to its original form.

Next, we **encrypt** `msg` by using the `pubKey` and we obtain as a result the following set of output: { `ciphertext`, `nonce`, `authTag`, `ciphertextPubKey` }. 

To **decrypt** the encrypted message, we use the data produced during the encryption { `ciphertext`, `nonce`, `authTag`, `ciphertextPubKey` }, along with the decryption `privateKey`. The result is the decrypted plaintext message. We use authenticated encryption \(GCM block mode\), so if the decryption key or some other parameter is incorrect, the decryption will fail with an **exception**.

Internally, the `encrypt_ECC(msg, pubKey)` function first generates an ephemeral **ECC key-pair** for the ciphertext and calculates the symmetric encryption shared ECC key `sharedECCKey = ciphertextPrivKey * pubKey`. This key is an EC point, so it is then transformed to **256-bit AES secret key** \(integer\) though hashing the point's `x` and `y` coordinates. Finally, the **AES-256-GCM** cipher \(from `pycryptodome`\) **encrypts** the message by the 256-bit shared secret key `secretKey` and produces as **output** `ciphertext` + `nonce` + `authTag`.

The `decrypt_ECC(encryptedMsg{ciphertext, nonce, authTag, ciphertextPubKey}, privKey)` function internally first calculates the symmetric encryption shared ECC key `sharedECCKey = privKey * ciphertextPubKey`. It is an EC point, so it should be first transformed to **256-bit AES secret key** though hashing the point's `x` and `y` coordinates. Then the **AES-256-GCM cipher** is used to **decrypt** the `ciphertext` + `nonce` + `authTag` by the 256-bit shared secret key `secretKey`. The produced output is the original plaintext message \(or an exception in case of incorrect decryption key or unmatching `authTag`\).

The version of Python we used was 3.9.6

Author: Shubham K Dathia and
Rajeshwar Singh
