.PHONY : clean

main : main.c
	gcc -o $@ $< -lm -std=c99 -g

run :
	./main core_gene_alignment.aln distance_matrix.txt

clean :
	rm -f main
