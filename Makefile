log:= thirdparty/log/log.h thirdparty/log/log.c

log_test: test/log_test.c $(log)
	gcc $^ -o $@ -I thirdparty
	./$@
	-rm $@