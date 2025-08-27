#目标文件: 依赖列表
#	命令1
#	命令2
#	命令3
# main: main.c hello.c hello.h
# 	gcc  main.c hello.c hello.h -o main
# 	./main
# 	rm main

#定义变量
objects := main.o
objects += hello.o

# 伪目文件，clean没有依赖列表，在创建clean文件后,会当成创建文件命令
.PHONY: clean

main.o: main.c hello.h
	gcc -c main.c -o main.o

hello.o: hello.c
	gcc -c hello.c -o hello.o

main: $(objects)
	gcc $objects -o main
	./main

clean: rm main.o rm hello.o rm main