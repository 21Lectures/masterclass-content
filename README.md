# Bitcoin course exercises

To streamline the virtual machine setup, we access the exercise notebooks through a Docker container with all the necessary dependencies installed.

## Prerequisites
You need
1. [git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git)
1. [docker](https://docs.docker.com/engine/install/ubuntu/) and [docker-compose](https://docs.docker.com/compose/install/)

Make sure that `docker run hello-world` runs through successfully.

## Setup
Afterwards, in the directory where this `README` is located, fetch the exercises with: 

```bash
git clone https://github.com/dspicher/code-demos exercises/bx

git clone https://github.com/dspicher/pb-exercises exercises/python
```

Finally, in the same directory, start the docker container with
```bash
docker-compose up
```
The final line of the output should contain a link of the form `http://127.0.0.1:8888/?token=<token>` where the Jupyter notebook is accessible.

The `exercises` folder where we have cloned the repositories is mounted into the container. Therefore, any changes you make in there will be reflected on your local file system. Feel free to test this by modifying some notebook files.

## If you have never used Jupyter notebooks
Make sure to familiarize yourself a bit with notebooks. Some introductory links you may find helpful:
 - https://towardsdatascience.com/a-beginners-tutorial-to-jupyter-notebooks-1b2f8705888a
 - https://realpython.com/jupyter-notebook-introduction/

## Python Exercises Index

 - Session 0
     - Python & Jupyter tips
     - Modulo arithmetic
     - Endianness, int-to-bytes
 - Session 1
     - EC field and group operations
 - Session 2
     - EC group operations
     - Public key encoding
     - Base58 addresses
 - Session 3
     - ECDSA signatures
     - Transaction parsing
 - Session 4
     - Script
     - Transaction serializing
     - Inputs, outputs, fees
     - Script validation
 - Session 5
     - P2PKH, P2SH addresses
     - Transaction & endorsement construction
     - Multisig Script
 - Session 6
     - Block parsing, serialization
     - Coinbase transactions
     - Target, difficulty, bits
     - PoW
 - Session 7
     - P2P
     - Header download
     - Merkle trees
 - Session 8
     - Bloom filters
     - SPV