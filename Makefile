log:= thirdparty/log/log.h thirdparty/log/log.c

log_test: test/log_test.c $(log)
	gcc $^ -o $@ -I thirdparty
	./$@
	-rm $@

cjson:= thirdparty/cjson/cJSON.h thirdparty/cjson/cJSON.c
cjson_test: $(cjson) test/cjson_test.c $(log)
	gcc $^ -o $@ -I thirdparty
	./$@
	-rm $@

app_common:= app/app_common.h app/app_common.c
app_common_test: $(app_common) test/app_common_test.c $(log)
	gcc $^ -o $@ -I thirdparty -Iapp
	./$@
	-rm $@