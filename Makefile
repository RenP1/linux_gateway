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

app_message:= app/app_message.h app/app_message.c
app_message_test: $(app_message) test/app_message_test.c $(log) $(app_common) $(cjson)
	gcc $^ -o $@ -I thirdparty -Iapp
	./$@
	-rm $@

mqtt_test: test/mqtt_test.c
	-gcc $^ -o $@ -lpaho-mqtt3c
	-./$@
	-rm $@

app_mqtt:= app/app_mqtt.h app/app_mqtt.c
app_mqtt_test: $(app_mqtt) test/app_mqtt_test.c $(log)
	gcc $^ -o $@ -I thirdparty -Iapp -lpaho-mqtt3c
	./$@
	-rm $@