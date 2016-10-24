CFLAGES =    -Wall -O                  #编译选项
OBJS = ./httpdispatch.o
CC = g++
TARGET = httpdispatch                  #生成的可执行文件
$(TARGET):$(OBJS)                      #TARGET目标，需要先生成OBJS目标
	$(CC) -o  $@  $^ $(CFLAGES)    #生成可执行文件
$(OBJS):%.o:%.cpp                      #将OBJS中所有扩展名为.o的文件替换成扩展名为.c的文件
	$(CC) -c $<  $(CFLAGES) -o $@  #采用CFLAGS指定的选项编译生成目标文件
clean:                                 #清理
	-$(RM) $(TARGET) $(OBJS)       #删除所有的目标文件和可执行文件
