create:= 01_create.c
create_test: $(create)
	gcc $^ -o $@
	./$@
	-rm $@

sum_sqr:= 02_sum_sqr.c
sum_sqr_test: $(sum_sqr)
	gcc $^ -o $@
	./$@
	-rm $@

cancle:= 03_cancle.c
cancle_test: $(cancle)
	gcc $^ -o $@
	./$@
	-rm $@

race_condition:= 04_race_condition.c
race_condition_test: $(race_condition)
	gcc $^ -o $@
	./$@
	-rm $@