Os arquivos de exemplo são `exemplo_1.txt` e `exemplo_2.txt`.

Para compilá-los, deve-se, primeiramente, executar `make` e, posteriormente, executar `./etapa7 < [nome do arquivo] > out.dot`.

Os grafos gerados contém dois vértices especiais: um vértice de início (o único sem rótulo); e um vértice de fim (com o rótulo `L1`). Os demais vértices são os blocos básicos do programa.

Comandos de retorno são modelados como saltos para `L1`. As expressões dos comandos de retorno são computadas, mas não são utilizadas.

