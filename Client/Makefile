cou=../RELEASE/moda/cou.o
stu=../RELEASE/moda/stu.o
home=../RELEASE/moda/home.o
stu-c=../RELEASE/moda/stu-cou.o
libmoda=../RELEASE/moda/libmoda.so 
rmfile=$(cou) $(stu) $(home) $(stu-c) $(libmoda)
all:$(cou) $(stu) $(home) $(stu-c) $(libmoda)
$(cou):cou.c 
	gcc -c $< -o $@ -fPIC  
$(stu):stu.c 
	gcc -c $< -o $@ -fPIC
$(home):home.c 
	gcc -c $< -o $@ -fPIC
$(stu-c):stu-cou.c 
	gcc -c $< -o $@ -fPIC
$(libmoda):$(cou) $(home) $(stu) $(stu-c)
	gcc  -shared -o $@  $^ 
.PHONY:clean
clean:
	rm $(rmfile) 

