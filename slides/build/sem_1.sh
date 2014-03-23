echo "entering build directory"
cd ./bin/sem_1

echo "copying files to build directory"
cp -R ../../seminarka_1/* ./

echo "1. pass..."
pdflatex 00_main.tex
echo "2. pass..."
pdflatex 00_main.tex
echo "3. pass..."
pdflatex 00_main.tex
echo "4. pass..."
pdflatex 00_main.tex

cp -f ./00_main.pdf ../out/sem_1.pdf
