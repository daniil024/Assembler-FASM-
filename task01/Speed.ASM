format PE console

entry Start

include 'win32a.inc'

section '.data' data readable writable
         strE db 'Please, write what you want to find: speed - 1 or distance - 2: ', 0
         str1 db 'Please enter distance: ', 0
         str2 db 'Please enter time: ', 0
         str3 db 'Please enter speed: ', 0
         strRes db 'Result = %d', 0

         spaceStr db '%d', 0

         D dd ?
         T dd ?
         S dd ?

         NULL = 0


section '.code' code readable executable
        Start:
               push strE
               call [printf]

               call [getch]

               cmp eax, 49
               jne notSpeed
                   push str1
                   call [printf]

                   push D
                   push spaceStr
                   call [scanf]

                   push str2
                   call [printf]

                   push T
                   push spaceStr
                   call [scanf]

                   mov eax, [D]
                   mov ecx, [T]
                   mov edx, 0

                   div ecx

                   push eax
                   push strRes
                   call [printf]

                   jmp finish
               notSpeed:


               push str2
               call [printf]

               push T
               push spaceStr
               call [scanf]

               push str3
               call [printf]

               push S
               push spaceStr
               call [scanf]

               mov eax, [T]
               mul [S]

               push eax
               push strRes
               call [printf]

        finish:

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










