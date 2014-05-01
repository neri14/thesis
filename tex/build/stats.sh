cd ./bin/out

pdfinfo thesis.pdf > stats.txt
pdftotext thesis.pdf
wc -w thesis.txt > word_count.txt