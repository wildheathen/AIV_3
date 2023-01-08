@echo off

if exist "bin"( RD /S /Q "bin" )

MD "bin"

clang.exe -o bin\list-test.exe -Iinclude test\main.c src\LinkedList_Impl.c
clang.exe -o bin\set-test.exe src\Set_Impl.c
clang.exe -o bin\DoublyLinkedList-test.exe src\DoublyLinkedList_Impl.c
clang.exe -o bin\Dictionary_Impl-test.exe src\Dictionary_Impl.c