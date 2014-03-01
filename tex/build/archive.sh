echo "entering build directory"
cd ./bin/archivemode

echo "copying files to build directory"
cp -R ../../src/* ./

sed "s/documentclass\[printmode\]{mgr}/documentclass\[archivemode\]{mgr}/" -i 00_main.tex

echo "1. pass..."
pdflatex 00_main.tex
echo "2. pass..."
pdflatex 00_main.tex
echo "3. pass..."
pdflatex 00_main.tex
echo "4. pass..."
pdflatex 00_main.tex

cp -f ./00_main.pdf ../out/archive.pdf
