add_test([=[FiniteStateMachine.InsertsNewNodes]=]  /Users/vismay/Code/DistSys/swim-protocol/build/fsa_test [==[--gtest_filter=FiniteStateMachine.InsertsNewNodes]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[FiniteStateMachine.InsertsNewNodes]=]  PROPERTIES WORKING_DIRECTORY /Users/vismay/Code/DistSys/swim-protocol/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  fsa_test_TESTS FiniteStateMachine.InsertsNewNodes)
