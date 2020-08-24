all: diff patch clean
diff: diff.o utility.o lcs.o ses.o create.o list.o queue.o print.o stats.o recursive.o
	cc diff.o utility.o lcs.o ses.o create.o list.o queue.o print.o stats.o recursive.o -o diff -Wall
patch: patch.o lines.o cp.o npatch.o
	cc patch.o lines.o cp.o npatch.o -o patch -Wall
diff.o: diff.c header.h
	cc -c diff.c -Wall
utility.o: utility.c header.h
	cc -c utility.c -Wall
lcs.o: lcs.c header.h
	cc -c lcs.c -Wall
ses.o: ses.c header.h
	cc -c ses.c -Wall
create.o: create.c header.h list.h
	cc -c create.c -Wall
list.o: list.c list.h queue.h
	cc -c list.c -Wall
queue.o: queue.c queue.h
	cc -c queue.c -Wall
print.o: print.c queue.h
	cc -c print.c -Wall
stats.o: stats.c header.h
	cc -c stats.c -Wall
recursive.o: recursive.c header.h
	cc -c recursive.c -Wall
patch.o: patch.c hpatch.h
	cc -c patch.c -Wall
lines.o: lines.c hpatch.h
	cc -c lines.c -Wall
cp.o: cp.c hpatch.h
	cc -c cp.c -Wall
npatch.o: npatch.c hpatch.h
	cc -c npatch.c -Wall
clean:
	rm *.o
