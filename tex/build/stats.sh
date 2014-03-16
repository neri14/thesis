cd ./bin/out

pdfinfo print.pdf > print_stats.txt
pdfinfo archive.pdf > archive_stats.txt
pdfinfo debug.pdf > debug_stats.txt

pdftotext print.pdf
pdftotext archive.pdf
pdftotext debug.pdf

wc -w print.txt > print_word_count.txt
wc -w archive.txt > archive_word_count.txt
wc -w debug.txt > debug_word_count.txt
