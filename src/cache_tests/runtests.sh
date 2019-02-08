echo "Running unit tests:"

for i in cache_tests/*_tests cache_tests/*_tests.exe
do
  if test -f $i
  then
    if $VALGRIND ./$i 2>> cache_tests/cache_tests.log
    then
      echo $i PASS
    else
      echo "ERROR in test $i: here's cache_tests/cache_tests.log"
      echo "-----"
      tail cache_tests/cache_tests.log
      exit 1
    fi
  fi
done

echo ""