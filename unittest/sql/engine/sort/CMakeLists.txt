#function(sort_unittest case)
#  ob_unittest(${ARGV})
#  target_sources(${case} PRIVATE ob_fake_table.h ob_fake_table.cpp)
#endfunction()
#
#sort_unittest(ob_specific_columns_sort_test)
#sort_unittest(ob_in_memory_sort_test)
#sort_unittest(ob_sort_test)
#sort_unittest(ob_merge_sort_test)
#sort_unittest(test_sort_impl)
