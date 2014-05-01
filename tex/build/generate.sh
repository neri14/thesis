echo "entering build directory"
cd ./bin/tmp

echo "copying files to build directory"
cp -R ../../src/* ./

echo "1. latex pass..."
latex LaTeX.tex
echo "bibtex pass..."
bibtex LaTeX.aux
echo "2. latex pass..."
latex LaTeX.tex
echo "3. latex pass..."
latex LaTeX.tex
echo "4. latex pass..."
latex LaTeX.tex
echo "5. latex pass..."
latex LaTeX.tex
echo "6. latex pass..."
latex LaTeX.tex

cp -f ./LaTeX.pdf ../out/thesis.pdf
