Gustavo Silveira Frehse - GRR20203927 14/02/23

# Rodando

Para compilar:

$ make

Para encriptografar:

$ ./cypher <senha> < <arquivo com texto limpo> > <arquivo saida criptografado>

Para descriptografar:

$ ./cypher -d <senha> < <arquivo criptografado> > <arquivo limpo>

# Algoritmo

O algoritmo funciona em blocos de 32 bytes.

Para criptografar, cada byte de entrada é associado a um peso (baseado na
senha). O bloco é então ordenado com base nos pesos.

Para descriptografar, é necessário encontrar a posição correta de cada
byte. Para isso a senha é 'comprimida', isso é, os valores da senha são
trocados por valores de [0, tamsenha), porém preservando a ordem entre eles.

Depois disso é facil achar o texto descriptografado, já que
    saida[i] = entrada[key[i]].

# Padding

O padding foi um problema já que o algoritmo funciona somente em blocos de
exatamente 32 bytes.

Assim sempre é adicionado um padding tal que o tamanho da entrada seja um
múltiplo de 32. Se a entrada já for múltiplo de 32 é adiconado 32 bytes
de padding.

No padding é adicionado um byte '-' seguido de '=' para completar os 32
bytes.

