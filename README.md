# sendByte
Tool to send bytes to a server

Built within the context of Datacommunication's option at BCIT's CST program with the intention of helping students debug their server.

## Requirements

- Capability to build the project
- OS that implements POSIX. Does not support windows although you may give it a go with WSL.

## instructions

! warning: Does not check for input errors. You break it, you pay it.

! warning: Make sure you place the bytes in the correct endianness if the server is expecting to receive data of size larger than a byte

1. run the exec. You can optionally enter the buffer size of bytes you want to send and receive as a cli arg : ./sendByte 400. Will default to 20.
1. enter ip address #.#.#.#
1. enter port #
1. enter bytes you want to send as numbers separated by a space: 123 121 3 3. The numbers will be parsed as ints and sent
1. server response will be printed in a likewise manner
1. ctl + C to terminate

## exit codes:

    enum {
        EXIT_DISCONN,
        EXIT_W_ERROR,
        EXIT_R_ERROR,
        EXIT_GETHOSTNAME_ERROR,
        EXIT_SOCKET_ERROR,
        EXIT_CONNECT_ERROR,
        EXIT_BAD_PORT_ERROR,
        EXIT_MEM_ALLOC_ERROR
    };
