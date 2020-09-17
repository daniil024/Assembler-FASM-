format PE console

entry Start

include 'win32a.inc'

section '.data' data readable writable
        resStr db 'Result: %d', 0
        str1 db 'ZF = 1, op result is FALSE', 0
        str2 db 'ZF = 0, op result is TRUE', 0

        A dw ?
        B dw ?
        C dw ?

        NULL = 0

section '.code' code readable executable
        Start:
                mov eax, 1010b
                test eax, 101b

                jz ifZFTrue

                push str2
                call [printf]

                jmp finish

                ifZFTrue:
                        push str1
                        call [printf]


                finish:
                        call [getch]

                        push NULL
                        call [ExitProcess]


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