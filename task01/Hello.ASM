format PE console

entry start

include 'win32a.inc'

section '.data' data readable writable
        strN db 'Please, enter your name: ', 0
        strS db 'Please, enter your surname: ', 0
        strH db 'Hi, %s %s, nice to see you again!', 0

        spaceStrN db '%s', 0
        spaceStrS db '%s', 0

        N rd 2
        S rd 2

        NULL = 0


section '.code' code readable executable
        start:
                push strN
                call [printf]

                push N
                push spaceStrN
                call [scanf]

                push strS
                call [printf]

                push S
                push spaceStrS
                call [scanf]

                push S
                push N
                push strH
                call [printf]

                call [getch]

                push NULL
                call [ExitProcess]

section '.idata' import data readable
        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll'

        import kernel,\
               ExitProcess, 'ExitProcess'

        import msvcrt,\
               printf, 'printf',\
               scanf, 'scanf',\
               getch, '_getch'