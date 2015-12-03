# encrypt

This is a simple, proof of concept implementation of [three-pass protocol](https://en.wikipedia.org/wiki/Three-pass_protocol) encryption. The encryption works as follows:

1. Sender multiplies every byte of its message by its key into an int
2. Sender sends the modified message to the receiver
3. Receiver multiplies every int of received message by its key
4. Receiver sends message back to sender
5. Sender divides every int of message by its key
6. Sender sends message back to receiver
7. Receiver divides every int of message by its key. 
8. The message is now in plaintext at the receivers end.

## compiling

Just call make:

    $ make

## running

To start the receiver:

    $ ./receiver <port> <key>

To start the sender:

    $ ./sender <host> <port> <key>
