format PE console

entry Start

include 'win32a.inc'

section '.data' data readable writeable

        str_inputSizeArr  db 'Please, input the number of elements - ', 0
        str_element       db 'Element [%d]: ', 0
        str_parseValue    db '%d', 0
        str_outValue      db '%d ', 0
        str_outputA       db 'Array A:', 10,  0
        str_outputB       db 'Array B:', 10, 0
        str_br1           db '[ ', 0
        str_br2           db ']', 0, 0ah

        arr_A        rd 100
        arr_B        rd 100
        size_A       dd 0
        size_B       dd 0
        i            dd ?
        tmp          dd ?
        tmp_st       dd ?
        st_help      dd ?

        NULL = 0

section '.code' code readable executable

;Просим пользователя ввести размер массива и его элементы
        readArr:
                push str_inputSizeArr
                call [printf]
                add esp, 4
                push size_A
                push str_parseValue
                call [scanf]
                add esp, 8

                mov eax, [size_A]
                cmp eax, 0
                jle readArr
                cmp eax, 100
                jge readArr

        vec:
                xor ecx, ecx
                mov ebx, arr_A

        arrLoop:
                mov [tmp], ebx
                cmp ecx, [size_A]
                jge fin_input

                mov [i], ecx
                push ecx
                push str_element
                call [printf]
                add esp, 8

                push ebx
                push str_parseValue
                call [scanf]
                add esp, 8

                mov ecx, [i]
                inc ecx
                mov ebx, [tmp]
                add ebx, 4
                jmp arrLoop

        fin_input:
                ret



;Находим нечетные элементы
        findOdd:
                xor ecx, ecx
                xor eax, eax
                mov ebx, arr_A
                mov edx, arr_B

        oddel:
                cmp ecx, [size_A]
                je finLoop

                mov [i], eax
                mov eax, [ebx]
                and eax, 1b
                cmp eax, 1
                je newel
                mov eax, [i]

        res:
                inc ecx
                add ebx, 4
                jmp oddel


        newel:
                mov eax, [i]
                inc eax
                mov [i], eax
                mov eax, [ebx]
                mov [edx], eax
                mov eax, [i]
                add edx, 4
                jmp res

        finLoop:
                mov [size_B], eax
                ret


;Выводим первоначальный массив
        A_Output:
                mov [tmp_st], esp
                xor ecx, ecx
                mov ebx, arr_A

        putVecLoop:
                mov [tmp], ebx
                cmp ecx, [size_A]
                je endOutputVector
                mov [i], ecx

                push dword [ebx]
                push str_outValue
                call [printf]

                mov ecx, [i]
                inc ecx
                mov ebx, [tmp]
                add ebx, 4
                jmp putVecLoop

        endOutputVector:
                push str_br2
                call [printf]
                mov esp, [tmp_st]
                ret


;Выводим получившийся массив
        B_Output:
                mov [tmp_st], esp
                xor eax, eax
                mov edx, arr_B

        newArrLoop:
                mov [tmp], edx
                cmp eax, [size_B]
                je finOut
                mov [i], eax

                push dword [edx]
                push str_outValue
                call [printf]

                mov eax, [i]
                inc eax
                mov edx, [tmp]
                add edx, 4
                jmp newArrLoop

        finOut:
                push str_br2
                call [printf]
                mov esp, [tmp_st]
                call [getch]

                push NULL
                call [ExitProcess]

;Main Method

         Start:
                call readArr

                call findOdd

                push str_outputA
                call [printf]

                push str_br1
                call [printf]

                mov [st_help], eax

                call A_Output

                mov eax, [st_help]

                push str_outputB
                call [printf]

                push str_br1
                call [printf]

                call B_Output

                ;Завершаем программу
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
               getch, '_getch',\
               scanf, 'scanf'
