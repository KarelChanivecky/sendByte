# sendByte
Tool to send bytes to a server

Built within the context of Datacommunication's option at BCIT's CST program with the intention of helping students debug their server.


## instructions

!warning: Does not check for input errors. You break it, you pay it.

1. run the exec. You can optionally enter the buffer size of bytes you want to send and receive as a cli arg : ./sendByte 400. Will default to 20.
1. enter ip address #.#.#.#
1. enter port #
1. enter bytes you want to send as numbers separated by a space: 123 121 3 3. The numbers will be parsed as ints and sent
1. server response will be printed in a likewise manner
