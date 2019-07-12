
for i in '1' '2' '3' '4' '5' '6'; do
  for t in 'c'; do
    valgrind --tool=memcheck --leak-check=full ./testCentralityMeasures graphs/$i.in $t
  done
done 
