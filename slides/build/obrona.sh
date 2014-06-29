echo "entering build directory"
cd ./bin/obr

echo "copying files to build directory"
cp -R ../../obrona/* ./

echo "1. pass..."
pdflatex 00_main.tex
echo "2. pass..."
pdflatex 00_main.tex
echo "3. pass..."
pdflatex 00_main.tex
echo "4. pass..."
pdflatex 00_main.tex

cp -f ./00_main.pdf ../out/obr.pdf
