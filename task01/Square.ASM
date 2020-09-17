format PE console

entry start

include 'win32a.inc'

section '.data' data readable writable
        strS db 'To calculate square of square enter the size of the side: ', 0
        strRes db 'Square = %d', 0

        size rd 1
        formatSize db '%d', 0

        NULL = 0


section '.code' code readable executable
        start:
              push strS
              call [printf]

              push size
              push formatSize
              call [scanf]

                 mov ecx, [size]
                 imul ecx, [size]

                 push ecx
                 push strRes
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





