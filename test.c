#include <stdlib.h>
#include <check.h>
#include <stdio.h>
#include <stdint.h>
#include "./include/tuple.h"
#include "./include/relation.h"
#include "./include/radix_sort.h"


START_TEST(test_tuple_function)
{
  tuple mytup;
  tuple_set_payload(&mytup, 15);
  int bool=(tuple_get_payload(&mytup)== 15);
  if(!bool){
    ck_abort_msg ("error");
  }
}
END_TEST

START_TEST(test_get_nth_key)
{
  tuple mytup;
  tuple_set_payload(&mytup, 15);
  uint64_t key = 18374686479671623680;
  tuple_set_key(&mytup, key);
  int byte = tuple_get_nth_byte(&mytup, 8);
  ck_assert_int_eq(byte,255);

  key = 71776119061217280;
  tuple_set_key(&mytup, key);
  byte = tuple_get_nth_byte(&mytup, 7);
  ck_assert_int_eq(byte,255);

  key = 280375465082880;
  tuple_set_key(&mytup, key);
  byte = tuple_get_nth_byte(&mytup, 6);
  ck_assert_int_eq(byte,255);

  key = 1095216660480;
  tuple_set_key(&mytup, key);
  byte = tuple_get_nth_byte(&mytup, 5);
  ck_assert_int_eq(byte,255);

  key = 4278190080;
  tuple_set_key(&mytup, key);
  byte = tuple_get_nth_byte(&mytup, 4);
  ck_assert_int_eq(byte,255);

  key = 16711680;
  tuple_set_key(&mytup, key);
  byte = tuple_get_nth_byte(&mytup, 3);
  ck_assert_int_eq(byte,255);

  key = 65280;
  tuple_set_key(&mytup, key);
  byte = tuple_get_nth_byte(&mytup, 2);
  ck_assert_int_eq(byte,255);

  key = 255;
  tuple_set_key(&mytup, key);
  byte = tuple_get_nth_byte(&mytup, 1);
  ck_assert_int_eq(byte,255);

  key = 255;
  tuple_set_key(&mytup, key);
  byte = tuple_get_nth_byte(&mytup, -1);
  ck_assert_int_eq(byte,0);
}
END_TEST

Suite* tuple_suite(void)
{
  Suite *s;
  TCase *tc_core;
  s = suite_create("Tuple");
  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_tuple_function);
  tcase_add_test(tc_core, test_get_nth_key);
  suite_add_tcase(s, tc_core);
  return s;
}


START_TEST(test_relation_init)
{
  relation *myrel;
  myrel = malloc(sizeof(relation));
  relation_init_data(myrel, "test_data1", 1, NORM_OFF);


  // relation_print(myrel);
  uint64_t key = tuple_get_key(&myrel->tuples[10]);
  uint64_t payload = tuple_get_payload(&myrel->tuples[10]);
  ck_assert_int_eq(key,362);
  ck_assert_int_eq(payload,10);

  key = tuple_get_key(&myrel->tuples[49999]);
  payload = tuple_get_payload(&myrel->tuples[49999]);
  ck_assert_int_eq(key,1395062755);
  ck_assert_int_eq(payload,49999);
  // ck_assert_int_eq(myrel->tuples[20].payload,14);

  relation_destroy(myrel);
  free(myrel);
}
END_TEST

START_TEST(test_relation_hist_psum)
{
  relation *myrel;
  myrel = malloc(sizeof(relation));
  relation_init_data(myrel, "test_data1", 1, NORM_OFF);

  int *hist = relation_get_hist(myrel, 0, 49999, 5);
  int *psum = get_psum_from_hist(hist);
  // print_psum(psum);
  // print_hist(hist);
  ck_assert_int_eq(hist[0],50000);
  ck_assert_int_eq(psum[0],0);
  ck_assert_int_eq(psum[1],50000);

  relation_destroy(myrel);
  free(myrel);
}
END_TEST

START_TEST(test_relation_norm_algorithm)
{
  relation *myrel, *norm_myrel;
  myrel = malloc(sizeof(relation));
  norm_myrel = malloc(sizeof(relation));

  relation_init_data(myrel, "test_data1", 1, NORM_OFF);
  relation_init_data(norm_myrel, "test_data1", 1, NORM_ON);

  denormalize_keys(norm_myrel);
  ck_assert_int_eq(myrel->tuples[10].key, norm_myrel->tuples[10].key);
  ck_assert_int_eq(myrel->tuples[10].payload, norm_myrel->tuples[10].payload);

  ck_assert_int_eq(myrel->tuples[0].key, norm_myrel->tuples[0].key);
  ck_assert_int_eq(myrel->tuples[100].key, norm_myrel->tuples[100].key);

  relation_destroy(myrel);
  relation_destroy(norm_myrel);
  free(myrel);
  free(norm_myrel);
}
END_TEST

Suite* relation_suite(void)
{
  Suite *s;
  TCase *tc_core;
  s = suite_create("Relation");
  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_relation_init);
  tcase_add_test(tc_core, test_relation_hist_psum);
  tcase_add_test(tc_core, test_relation_norm_algorithm);
  suite_add_tcase(s, tc_core);
  return s;
}


START_TEST(test_radix_sort_right_order)
{
  relation *myrel;
  myrel = malloc(sizeof(relation));

  relation_init_data(myrel, "test_data1", 1, NORM_OFF);
  // RadixSort_Init(myrel);
  uint64_t min = myrel->tuples[0].key;
  uint64_t max = myrel->tuples[0].key;

  for(int i=0; i<50000; i++){

    if(min>tuple_get_key(&myrel->tuples[i]))
    {
      min=tuple_get_key(&myrel->tuples[i]);
    }
    if(max<tuple_get_key(&myrel->tuples[i]))
    {
      max=tuple_get_key(&myrel->tuples[i]);
      // printf("%lu\n",myrel->tuples[i].key);
      //
      // printf("%lu\n",myrel->tuples[i].payload);
    }
  }

  RadixSort_Init(myrel);

  // relation_print_range(myrel, 50000-10, 50000);
  ck_assert_int_eq(myrel->tuples[0].key, min);
  ck_assert_int_eq(myrel->tuples[50000-1].key, max);
  uint64_t key, prev_key;
  prev_key = myrel->tuples[0].key;
  for(int i=0; i<50000; i++){
    key = myrel->tuples[i].key;
    ck_assert_int_le(prev_key, key); //previous key <= next key
  }
  //
  // ck_assert_int_eq(myrel->tuples[0].key, norm_myrel->tuples[0].key);
  // ck_assert_int_eq(myrel->tuples[100].key, norm_myrel->tuples[100].key);

  relation_destroy(myrel);
  free(myrel);
}
END_TEST

Suite* radix_sort_suite(void)
{
  Suite *s;
  TCase *tc_core;
  s = suite_create("RadixSort");
  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_radix_sort_right_order);
  suite_add_tcase(s, tc_core);
  return s;
}

int main(void){
  int number_failed;
  Suite* s;
  Suite* rel_suite;
  Suite* rad_sort_suite;

  s = tuple_suite();
  rel_suite = relation_suite();
  rad_sort_suite = radix_sort_suite();

  SRunner *sr, *sr_rel, *sr_rad_sort;
  sr = srunner_create(s);
  sr_rel = srunner_create(rel_suite);
  sr_rad_sort = srunner_create(rad_sort_suite);

  srunner_run_all(sr, CK_NORMAL);
  srunner_run_all(sr_rel, CK_NORMAL);
  srunner_run_all(sr_rad_sort, CK_NORMAL);

  number_failed = srunner_ntests_failed(sr);
  number_failed += srunner_ntests_failed(sr_rel);
  number_failed += srunner_ntests_failed(sr_rad_sort);

  srunner_free(sr);
  srunner_free(sr_rel);
  srunner_free(sr_rad_sort);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
  // return 0;
}
